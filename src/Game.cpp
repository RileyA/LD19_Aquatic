#include "stdafx.h"
#include "Game.h"

#include "State.h"
#include "Bucket.h"

// define this here
Game* Game::mInstance = 0;

Game::Game()
    :mInitialized(false),
	 mShutdown(false),
	 mEndState(false),
     mLog(true),
     mFileLog(true),
     mLogFile("LD19.log"),
     mLastTime(0),
     mDeltaTime(0.f),
     mTimeSpeed(1.f),
	 mGraphicsManager(0)
{
    assert(!mInstance);// only one of these should ever exist...
    mInstance = this;
	mConsoleIn = 0;
	mConsoleActive = false;
	mHistoryPos = 0;
	srand(time(0));
}
//---------------------------------------------------------------------------

Game::~Game()
{
    deinit();
}
//---------------------------------------------------------------------------

Game* Game::getPtr()
{
    return mInstance;
}
//---------------------------------------------------------------------------

Game& Game::getRef()
{
    return *mInstance;
}
//---------------------------------------------------------------------------

void Game::init()
{
    if(!mInitialized)
    {
        if(!mLogFile.is_open())
        {
            mFileLog = false;
        }
        else if(mLogFile.is_open()&&!mFileLog)
        {
            mLogFile.close();
        }

		mPhysicsManager = new PhysicsManager();

		mGraphicsManager = new GraphicsManager();
		mGraphicsManager->init(1024,768,true,false);

		mAudio = irrklang::createIrrKlangDevice();

		registerType<GraphicsManager>("GraphicsManager");
		registerFunction(&GraphicsManager::setFog,"setFog");

		registerType<Game>("Game");
		registerFunction(&Game::getPtr,"getGame");
		registerFunction(&Game::log,"log");
		registerFunction(&Game::getGraphicsManager,"getGfx");

		registerType<Signal>("Signal");
		registerType<Slot>("Slot");

		registerType<State>("State");
		registerFunction(&State::getSignal,"getSignal");
		registerFunction(&State::getSlot,"getSlot");

		runString("getGame().log(\"Scripting system functional!\");");

		mInputMgr = InputManager::getSingletonPtr();
		mInputMgr->initialise(mGraphicsManager->getWindow());
		mInputMgr->addKeyListener(this,"GameKeyListener");
		mInputMgr->addMouseListener(this,"GameMouseListener");
		//mInputMgr->addJoystickListener( this, "ListenerName" );

		Gorilla::Rectangle* rect = mGraphicsManager->getLayer(14)->createRectangle(0,570,1024,727-530);
		rect->background_colour(Ogre::ColourValue(0.4,0.4,0.4,0.6));

		rect = mGraphicsManager->getLayer(14)->createRectangle(0,727,1024,30);
		rect->background_colour(Ogre::ColourValue(0.4,0.4,0.4,0.8));

		mConsoleIn = mGraphicsManager->getLayer(15)->createMarkupText(14,10,730,"%6> ");
		for(uint i=1;i<8;++i)
		{
			mConsoleOut.push_back(mGraphicsManager->getLayer(15)->createMarkupText(14,10,720-i*20,"~"));
		}

		mGraphicsManager->getLayer(14)->hide();
		mGraphicsManager->getLayer(15)->hide();

		createBucket("Default");

        log("Basic subsystems initialized!");

        mInitialized = true;
    }
}
//---------------------------------------------------------------------------

void Game::deinit()
{
    if(mInitialized)
    {
		mConsoleOut.clear();
		mConsoleIn = 0;

		delete mGraphicsManager;
		mGraphicsManager = 0;

		mAudio->drop();

        log("Basic subsystems deinitialized!");

        mInitialized = false;
    }
}
//---------------------------------------------------------------------------

void Game::go()
{
    if(mInitialized)
    {
		while(!mStates.empty()&&!mShutdown)
		{
			mEndState = false;
			mStates[0]->init();
			mInputMgr->addKeyListener(mStates[0],"CurrentStateKeyListener");
			mInputMgr->addMouseListener(mStates[0],"CurrentStateMouseListener");

			mLastTime = clock();
			while(mStates[0]->update(mDeltaTime)&&!mEndState&&!mShutdown)
			{
				long now = clock();
				long delta = now - mLastTime;
				mLastTime = now;
				mDeltaTime = static_cast<float>(delta)/1000.f;

				std::map<String,Bucket*>::iterator it = mBuckets.begin();
				for(it;it!=mBuckets.end();++it)
					it->second->update(mDeltaTime*mTimeSpeed);

				mInputMgr->capture();
				mPhysicsManager->update(mDeltaTime*mTimeSpeed);
				mGraphicsManager->update();
			}

			mStates[0]->deinit();
			mInputMgr->removeKeyListener("CurrentStateKeyListener");
			mInputMgr->removeMouseListener("CurrentStateMouseListener");

			std::map<String,Bucket*>::iterator i = mBuckets.begin();
			for(i;i!=mBuckets.end();++i)
				delete i->second;
			mBuckets.clear();
			createBucket("Default");

			delete mStates[0];
			mStates.erase(mStates.begin());

			mGraphicsManager->endState();
			mPhysicsManager->deinit();
		}

		for(uint i=0;i<mStates.size();++i)
		{
			delete mStates[i];
		}
		mStates.clear();
    }
}
//---------------------------------------------------------------------------

void Game::log(String message)
{
    if(mLog)
    {
        String timeStamp = getTimeStamp();
        std::cout<<timeStamp<<": "<<message<<"\n";

        if(mFileLog&&mLogFile.is_open())
        {
            mLogFile<<timeStamp<<": "<<message<<"\n";
            mLogFile.flush();
        }
    }
	pushConsole(message);
}
//---------------------------------------------------------------------------

Real Game::getDeltaTime()
{
    return mDeltaTime;
}
//---------------------------------------------------------------------------

Real Game::getTimeSpeed()
{
    return mTimeSpeed;
}
//---------------------------------------------------------------------------

void Game::setTimeSpeed(Real speed)
{
    if(speed>=0.f)
    {
        mTimeSpeed = speed;
    }
}
//---------------------------------------------------------------------------

String Game::getTimeStamp()
{
    uint now = clock() / 1000;
    uint hours = now / 3600;
    now = now % 3600;
    uint minutes = now / 60;
    now = now % 60;
    uint seconds = now;

    // format = "HH:MM:SS"
    std::stringstream stamp;

    if(hours<10)
        stamp<<"0";
    stamp<<hours<<":";
    if(minutes<10)
        stamp<<"0";
    stamp<<minutes<<":";
    if(seconds<10)
        stamp<<"0";
    stamp<<seconds;

    String out;
    stamp>>out;

    return out;
}
//---------------------------------------------------------------------------

void Game::runString(String script)
{
	try
	{
		mChai.eval(script);
	}
	catch(chaiscript::Eval_Error e)
	{
		Game::getPtr()->log(String("Script Error: ")+e.what());
	}
}
//---------------------------------------------------------------------------

void Game::runFile(String filename)
{
	try
	{
		mChai.eval_file(filename);
	}
	catch(chaiscript::Eval_Error e)
	{
		Game::getPtr()->log(String("Script Error: ")+e.what());
	}
}
//---------------------------------------------------------------------------

void Game::addState(State* state)
{
	mStates.push_back(state);
}
//---------------------------------------------------------------------------

GraphicsManager* Game::getGraphicsManager()
{
	return mGraphicsManager;
}
//---------------------------------------------------------------------------

bool Game::keyPressed(const OIS::KeyEvent &evt) 
{
	if(mConsoleIn&&mConsoleActive&&evt.key!=OIS::KC_RETURN)
	{
		char key = translateWin32Text(evt.key);
		String str = std::string(&key);
		mConsoleIn->text(mConsoleIn->text()+str);
		if(evt.key==OIS::KC_BACK&&mConsoleIn->text().length()>5)
			mConsoleIn->text(mConsoleIn->text().substr(0,mConsoleIn->text().length()-2));
	}
    return true;
}
//---------------------------------------------------------------------------

bool Game::keyReleased(const OIS::KeyEvent &evt) 
{
    //if (evt.key == OIS::KC_ESCAPE)
	//	mShutdown = true;
	if(evt.key==OIS::KC_END)
		mEndState = true;
	else if(evt.key==OIS::KC_GRAVE)
	{
		mConsoleActive = !mConsoleActive;
		if(mConsoleActive)
		{
			mGraphicsManager->getLayer(14)->show();
			mGraphicsManager->getLayer(15)->show();
		}
		else
		{
			mGraphicsManager->getLayer(14)->hide();
			mGraphicsManager->getLayer(15)->hide();
		}
	}
	else if(evt.key == OIS::KC_RETURN&&mConsoleActive)
	{
		String script = mConsoleIn->text().substr(4,mConsoleIn->text().length()-1);
		if(mHistoryPos!=mConsoleHistory.size()-1)
		{
			mConsoleHistory.push_back(mConsoleIn->text());
		}
		mHistoryPos = -1;
		pushConsole("%6"+script);
		runString(script);
		mConsoleIn->text("%6> ");
	}
	else if(evt.key == OIS::KC_UP&&mConsoleActive&&mConsoleHistory.size()>0)
	{
		if(mHistoryPos==-1)
		{
			mHistoryPos = mConsoleHistory.size()-1;
			mConsoleIn->text(mConsoleHistory[mHistoryPos]);
		}
		else
		{
			if(mHistoryPos==0)
			{
				mHistoryPos = -1;
				mConsoleIn->text("%6> ");
			}
			else
			{
				--mHistoryPos;
				mConsoleIn->text(mConsoleHistory[mHistoryPos]);
			}
		}
	}
	else if(evt.key == OIS::KC_DOWN&&mConsoleActive&&mConsoleHistory.size()>0)
	{
		if(mHistoryPos==mConsoleHistory.size()-1)
		{
			mHistoryPos = -1;
			mConsoleIn->text("%6> ");
		}
		else
		{
			mHistoryPos++;
			mConsoleIn->text(mConsoleHistory[mHistoryPos]);
		}
	}
    return true;
}
//---------------------------------------------------------------------------

bool Game::mouseMoved(const OIS::MouseEvent &evt)
{
	return true;
}
//---------------------------------------------------------------------------

bool Game::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID btn)
{
	return true;
}
//---------------------------------------------------------------------------

bool Game::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID btn)
{
	return true;
}
//---------------------------------------------------------------------------

void Game::pushConsole(String out)
{
	if(mConsoleIn)
	{
		String temp = mConsoleOut[0]->text();
		mConsoleOut[0]->text(out);
		for(uint i=1;i<mConsoleOut.size();++i)
		{
			String temp2 = mConsoleOut[i]->text();
			mConsoleOut[i]->text(temp);
			temp = temp2;
		}
	}
}

void Game::registerObject(GameObject* obj,String name,String bucket)
{
	if(mBuckets.find(bucket)!=mBuckets.end())
	{
		mBuckets[bucket]->pushObject(obj,name);
	}
}

void Game::createBucket(String name)
{
	mBuckets[name] = new Bucket(name);
}