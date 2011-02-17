#include "stdafx.h"
#include "TestState.h"

#include "InputManager.h"

#include "Game.h"
#include "Treasure.h"
#include "Beacon.h"

int BuyOption::y = 0;

TestState::TestState()
{

}

TestState::~TestState()
{

}

void TestState::init()
{
	
	sensitivity = 1.f;

	std::ifstream file;
	file.open("sensitivity.txt");
	if(file.is_open())
	{
		file>>sensitivity;
		file.close();
	}


	idle = Game::getPtr()->mAudio->play2D("media/audio/idle.wav",true,false,true);
	motor = Game::getPtr()->mAudio->play2D("media/audio/motor.wav",true,false,true);
	idle->setVolume(0.65f);
	energyRate = 1.f;
	cashingOut = false;
	cashingStill = true;
	cashTimer = 0.2f;
	buying = false;
	energy = 1.f;
	currentCash = 2500;
	beacons = 15;
	lightsOn = true;
	nrBeacons  = 0;
	Game* game = Game::getPtr();
	game->createBucket("grids");
	game->createBucket("caches");
	game->getPhysicsManager()->init();
	GraphicsObject* obj = game->getGraphicsManager()->createGraphicsObject("Base.mesh");
	//game->getGraphicsManager()->createGraphicsObject("Chest01.mesh")->setPosition(Ogre::Vector3(0,-10,20));

	unsigned short x = 16384;
	unsigned short z = 16384;

	maxWeight = 250;// 250kg to begin with
	currentWeight = 0;

	 maxSpeed = 7.25f;
	 minSpeed = 1.83f;
	 speedRate = 2.38f;
	 currentSpeed = 0.f;

	 engineOn = true;


	lastPos = Ogre::Vector3(0,0,0);
	lastSpeed = 0.f;

	mGrids[x][z] = new Grid("Grid_"+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(z),"grids",x,z,0);
	//mGrids[x+1][z] = new Grid("Grid_"+Ogre::StringConverter::toString(x+1)+"_"+Ogre::StringConverter::toString(z),"Default",x+1,z,0);
	//mGrids[x+1][z-1] = new Grid("Grid_"+Ogre::StringConverter::toString(x+1)+"_"+Ogre::StringConverter::toString(z-1),"Default",x+1,z-1);
	//mGrids[x+1][z+1] = new Grid("Grid_"+Ogre::StringConverter::toString(x+1)+"_"+Ogre::StringConverter::toString(z+1),"Default",x+1,z+1,0);
	//mGrids[x-1][z] = new Grid("Grid_"+Ogre::StringConverter::toString(x-1)+"_"+Ogre::StringConverter::toString(z),"Default",x-1,z);
	//mGrids[x-1][z+1] = new Grid("Grid_"+Ogre::StringConverter::toString(x-1)+"_"+Ogre::StringConverter::toString(z+1),"Default",x-1,z+1);
	//mGrids[x-1][z-1] = new Grid("Grid_"+Ogre::StringConverter::toString(x-1)+"_"+Ogre::StringConverter::toString(z-1),"Default",x-1,z-1,0);
	//mGrids[x][z-1] = new Grid("Grid_"+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(z-1),"Default",x,z-1);
	//mGrids[x][z+1] = new Grid("Grid_"+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(z+1),"Default",x,z+1);
	
	//mGrids[x][z] = new Grid("Grid_"+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(z),"Default",x,z);
	//mGrids[x][z] = new Grid("Grid_"+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(z),"Default",x,z);
	//mGrids[x][z] = new Grid("Grid_"+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(z),"Default",x,z);

	Ogre::SceneManager* mgr = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager");
	mgr->setFog(Ogre::FOG_LINEAR,Ogre::ColourValue(44.f/455.f,90.f/455.f,93.f/455.f),0.f,5.f,40.f);

	// create hud:
	Gorilla::Layer* hud = Game::getPtr()->getGraphicsManager()->getLayer(0);
	Gorilla::Rectangle* energyBar2 = hud->createRectangle(255,650,512,40);
	energyBar = hud->createRectangle(255,650,128,40);
	energyBar->background_colour(Ogre::ColourValue(0.8f,0.2f,0.2f,0.5f));
	energyBar2->no_background();
	energyBar2->border(2,Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));

	fpsText = hud->createMarkupText(14,710,50,"%5Weight: 00");
	depthText = hud->createMarkupText(14,710,95,"%5Depth: 00");
	speedText = hud->createMarkupText(14,710,130,"%5Speed: 00");
	//hud->createMarkupText(14,750,65,"%5 Value: $0");

	pickText = hud->createCaption(9,512-200,420,"Crate");
	pickWeight = hud->createCaption(9,512-200,438,"Weight: 50 kg");
	pickValue = hud->createCaption(9,512-200,456,"Value: ~$500");
	pickContext = hud->createCaption(9,512-200,474,"Too Heavy!");

	cashOut = hud->createCaption(14,512-200,150,"Press SPACE to recharge and sell items");
	cashOut2 = hud->createCaption(14,512-200,200,"Or ENTER to buy stuff");
	cashOut->width(400);
	cashOut2->width(400);
	cashOut->height(30);
	cashOut->colour(Ogre::ColourValue(0.3,0.9,0.45));
	cashOut2->colour(Ogre::ColourValue(0.3,0.9,0.45));
	cashOut->align(Gorilla::TextAlign_Centre);
	cashOut2->align(Gorilla::TextAlign_Centre);
	cashOut->text("");
	cashOut2->text("");

	pickText->align(Gorilla::TextAlign_Centre);
	pickText->width(400);
	pickText->height(18);
	pickText->colour(Ogre::ColourValue(0.3,0.9,0.45));

	pickWeight->align(Gorilla::TextAlign_Centre);
	pickWeight->width(400);
	pickWeight->height(18);
	pickWeight->colour(Ogre::ColourValue(0.3,0.9,0.45));

	pickValue->align(Gorilla::TextAlign_Centre);
	pickValue->width(400);
	pickValue->height(18);
	pickValue->colour(Ogre::ColourValue(0.3,0.9,0.45));

	pickContext->align(Gorilla::TextAlign_Centre);
	pickContext->width(400);
	pickContext->height(18);
	//pickContext->colour(Ogre::ColourValue(0.3,0.9,0.45));
	pickContext->colour(Ogre::ColourValue(0.9,0.4,0.3));

	cashText = hud->createMarkupText(14,35,50,"%5Cash: $"+Ogre::StringConverter::toString(currentCash));
	engineText = hud->createMarkupText(14,35,95,"%5Engine : On (Q)");
	lightText = hud->createMarkupText(14,35,130,"%5Lights : On (E)");
	beaconText = hud->createMarkupText(14,35,165,"%5Beacons: 5 (F):");

	hud->createRectangle(512-15,369,30,30)->background_image("ret");

	Gorilla::Rectangle* rec;
	rec = hud->createRectangle(20,20,5,50);
	rec->background_colour(Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));
	rec = hud->createRectangle(25,20,45,5);
	rec->background_colour(Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));

	rec = hud->createRectangle(998,20,5,50);
	rec->background_colour(Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));
	rec = hud->createRectangle(953,20,45,5);
	rec->background_colour(Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));

	rec = hud->createRectangle(998,697,5,50);
	rec->background_colour(Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));
	rec = hud->createRectangle(953,742,45,5);
	rec->background_colour(Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));

	rec = hud->createRectangle(20,697,5,50);
	rec->background_colour(Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));
	rec = hud->createRectangle(25,742,45,5);
	rec->background_colour(Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));

	//hud->createRectangle(30,30,30,30)->background_image("rec");

	//energyBar->border_colour(Ogre::ColourValue(0.2f,0.8f,0.4f,0.75f));

	obj->setPosition(Ogre::Vector3(0,-10,0));
	//txt = game->getGraphicsManager()->getLayer(0)->createMarkupText(14,10,400,"%2> ");
	//txt->background(Ogre::ColourValue(1.f,0.f,0.f));
	//txt->_redraw();

	mPlayer = game->getPhysicsManager()->createSphere(2.4f,Ogre::Vector3(0,0,0));
	mPlayer->setCollisionGroup(COLLISION_GROUP_1|COLLISION_GROUP_15);
	mPlayer->setPosition(Ogre::Vector3(0,-5,22));
	//mPlayer->setAngularFactor(Ogre::Vector3(0,0,0));

	mLandingLights = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->createBillboardSet(20);
	mLandingLights->setMaterialName("Light");
	mLandingLights->setDefaultDimensions(1.15f,1.15f);
	mLandingLights->createBillboard(Ogre::Vector3(0,0,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(0,10,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(0,20,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(0,30,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(0,40,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(0,50,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(0,60,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(0,70,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(-10,-9.8,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(10,-9.8,0),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(0,-9.8,-10),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(0,-9.8,10),Ogre::ColourValue(1,0,0));

	mLandingLights->createBillboard(Ogre::Vector3(7,-9.8,7),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(7,-9.8,-7),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(-7,-9.8,-7),Ogre::ColourValue(1,0,0));
	mLandingLights->createBillboard(Ogre::Vector3(-7,-9.8,7),Ogre::ColourValue(1,0,0));


	mLight = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->createBillboardSet(1);
	mLight->setDefaultDimensions(1.f,1.f);
	mLight->setMaterialName("Light2");
	mLight->createBillboard(Ogre::Vector3(0,0,-3),Ogre::ColourValue(1,0,0));
	game->getGraphicsManager()->camRoll->attachObject(mLight);

	Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->getRootSceneNode()->attachObject(mLandingLights);

	// create cash in screen

	Gorilla::Layer* blackout = Game::getPtr()->getGraphicsManager()->getLayer(1);
	Gorilla::Rectangle* re = blackout->createRectangle(0,0,1024,768);
	re->background_gradient(Gorilla::Gradient_NorthSouth,Ogre::ColourValue(0.1,0.2,0.4,0.85),Ogre::ColourValue(0.01,0.02,0.04,0.7));//->background_colour(Ogre::ColourValue(0.1,0.2,0.4));
	Gorilla::Layer* ci = Game::getPtr()->getGraphicsManager()->getLayer(3);
	cashValCI = ci->createMarkupText(24,50,55,"Net Balance: "+Ogre::StringConverter::toString(currentCash));
	prof = ci->createMarkupText(24,600,55,"Profit: 0.00");

	Gorilla::Caption* h1 = ci->createCaption(24,212,140,"Item:");
	Gorilla::Caption* h2 = ci->createCaption(24,612,140,"Sold For:");

	h1->width(150);
	h2->width(160);
	h1->align(Gorilla::TextAlign_Centre);
	h2->align(Gorilla::TextAlign_Centre);
	h1->colour(Ogre::ColourValue(0.8,0.6,0.3));
	h2->colour(Ogre::ColourValue(0.8,0.6,0.3));

	for(uint i=0;i<10;++i)
	{
		names.push_back(ci->createCaption(24,212,200+i*50,"."));
		values.push_back(ci->createCaption(24,612,200+i*50,"."));
		values[values.size()-1]->colour(Ogre::ColourValue(0.7,0.9,0.8));
		names[names.size()-1]->colour(Ogre::ColourValue(0.8,0.7,0.6));
		//names[names.size()-1]->colour
	}

	ci->createCaption(24,50,710,"Press SPACE to go back.");

	Gorilla::Layer* buylayer = Game::getPtr()->getGraphicsManager()->getLayer(2);

	//buylayer->createMarkupText(24,75,50, "$10000    Speed      %:yesup%  %:yesup%  %:noup%  %:noup%")->width(1000);
	//buylayer->createMarkupText(24,75,100,"$10000    Cargo      %:yesup%  %:yesup%  %:noup%  %:noup%")->width(1000);
	//buylayer->createMarkupText(24,75,150,"$10000    Energy     %:yesup%  %:yesup%  %:noup%  %:noup%")->width(1000);
	//buylayer->createMarkupText(24,75,200,"$10000    Light      %:yesup%  %:yesup%  %:noup%  %:noup%")->width(1000);
	buylayer->createCaption(24,50,700,"Press ENTER to buy. SPACE to go back.")->width(1000);

	cashBuy = buylayer->createMarkupText(24,40,32,"%6Cash:  $"+Ogre::StringConverter::toString(currentCash));
	beaconBuy = buylayer->createMarkupText(24,600,32,"Beacons:  "+Ogre::StringConverter::toString(beacons));
	//beaconBuy->text("Beacons: "+Ogre::StringConverter::toString(beacons));


	mOptions.push_back(new BuyOption("Speed"));
	mOptions[mOptions.size()-1]->prices.push_back(2000);
	mOptions[mOptions.size()-1]->prices.push_back(5000);
	mOptions[mOptions.size()-1]->prices.push_back(10000);
	mOptions[mOptions.size()-1]->prices.push_back(25000);
	mOptions[mOptions.size()-1]->createText();

	mOptions.push_back(new BuyOption("Cargo"));
	mOptions[mOptions.size()-1]->prices.push_back(2000);
	mOptions[mOptions.size()-1]->prices.push_back(5000);
	mOptions[mOptions.size()-1]->prices.push_back(10000);
	mOptions[mOptions.size()-1]->prices.push_back(25000);
	mOptions[mOptions.size()-1]->createText();

	mOptions.push_back(new BuyOption("Energy"));
	mOptions[mOptions.size()-1]->prices.push_back(2000);
	mOptions[mOptions.size()-1]->prices.push_back(5000);
	mOptions[mOptions.size()-1]->prices.push_back(10000);
	mOptions[mOptions.size()-1]->prices.push_back(25000);
	mOptions[mOptions.size()-1]->createText();

	mOptions.push_back(new BuyOption("Lamp"));
	mOptions[mOptions.size()-1]->prices.push_back(2000);
	mOptions[mOptions.size()-1]->prices.push_back(5000);
	mOptions[mOptions.size()-1]->prices.push_back(10000);
	mOptions[mOptions.size()-1]->prices.push_back(25000);
	mOptions[mOptions.size()-1]->createText();

	mOptions.push_back(new BuyOption("5x Beacons"));
	mOptions[mOptions.size()-1]->prices.push_back(300);
	mOptions[mOptions.size()-1]->createText();

	DEATH = hud->createCaption(24,112,200,"");
	DEATH->width(800);
	DEATH->colour(Ogre::ColourValue(1,0.2,0.22));
	DEATH->align(Gorilla::TextAlign_Centre);


	//mOptions.push_back(new BuyOption("Sonar"));
	//mOptions[mOptions.size()-1]->prices.push_back(50000);
	//mOptions[mOptions.size()-1]->createText();

	//mOptions.push_back(new BuyOption("Backup Battery"));
	//mOptions[mOptions.size()-1]->prices.push_back(50000);
	//mOptions[mOptions.size()-1]->createText();

	//mOptions.push_back(new BuyOption("Compass"));
	//mOptions[mOptions.size()-1]->prices.push_back(10000);
	//mOptions[mOptions.size()-1]->createText();

}

void TestState::deinit()
{
	for(uint i=0;i<mOptions.size();++i)
	{
		delete mOptions[i];
	}
	mOptions.clear();
}

bool TestState::update(Real delta)
{
	// state machine? ha!
	if(!cashingOut&&!buying)
	{
		Game::getPtr()->getGraphicsManager()->getLayer(0)->show();
		Game::getPtr()->getGraphicsManager()->getLayer(3)->hide();
		Game::getPtr()->getGraphicsManager()->getLayer(2)->hide();
		Game::getPtr()->getGraphicsManager()->getLayer(1)->hide();

		// camera motion
		Ogre::Vector3 motion = Ogre::Vector3(isKeyDown(OIS::KC_D)-isKeyDown(OIS::KC_A),
			0,isKeyDown(OIS::KC_S)-isKeyDown(OIS::KC_W));
		motion.normalise();
		motion = Game::getPtr()->getGraphicsManager()->mCamera->getDerivedOrientation()*motion;
		//Game::getPtr()->getGraphicsManager()->camPos->translate(motion*delta*10.f);
		Game::getPtr()->getGraphicsManager()->camPos->setPosition(mPlayer->getPosition());
		if(motion.length()<0.5f||!engineOn)
		{
			motion = mPlayer->getVelocity()*0.99f;
			currentSpeed = 0.f;
			mPlayer->wakeup();
			if(!engineOn)
			{
				motion.y*=0.9f;
				motion+=Ogre::Vector3(0,-0.2,0);
			}
			mPlayer->setVelocity(motion);
			motor->setVolume(motion.length()/maxSpeed);

		}
		else
		{
			if(currentSpeed<minSpeed)
			{
				currentSpeed = minSpeed;
				if(mPlayer->getVelocity().length()>currentSpeed)
					currentSpeed = mPlayer->getVelocity().length();
			}
			currentSpeed+=delta*speedRate;
			if(currentSpeed>(maxSpeed-(maxSpeed/3.65)*(1.f*currentWeight/maxWeight)))
				currentSpeed = (maxSpeed-(maxSpeed/3.65f)*(1.f*currentWeight/maxWeight));
			//std::cout<<"Speed: "<<currentSpeed<<"\n";
			float factor = 1.f;

			motor->setVolume(currentSpeed/maxSpeed);
			
			mPlayer->wakeup();
			mPlayer->setVelocity(motion*currentSpeed*factor);
		}


		OIS::Mouse* ms = Game::getPtr()->getInputManager()->getMouse();
		float yawFactor = -ms->getMouseState().X.rel*0.015f*engineOn*sensitivity;
		float pitchFactor = -ms->getMouseState().Y.rel*0.015f*engineOn*sensitivity;

		if(Game::getPtr()->getGraphicsManager()->pitch+pitchFactor>Game::getPtr()->getGraphicsManager()->pichLimit)
		{
			pitchFactor = Game::getPtr()->getGraphicsManager()->pichLimit-Game::getPtr()->getGraphicsManager()->pitch;
			Game::getPtr()->getGraphicsManager()->pitch = Game::getPtr()->getGraphicsManager()->pichLimit;
		}
		else if(Game::getPtr()->getGraphicsManager()->pitch+pitchFactor<-Game::getPtr()->getGraphicsManager()->pichLimit)
		{
			pitchFactor = -Game::getPtr()->getGraphicsManager()->pichLimit-Game::getPtr()->getGraphicsManager()->pitch;
			Game::getPtr()->getGraphicsManager()->pitch = -Game::getPtr()->getGraphicsManager()->pichLimit;		
		}
		else
		{
			Game::getPtr()->getGraphicsManager()->pitch+=pitchFactor;
		}
		

		Game::getPtr()->getGraphicsManager()->camYaw->yaw(Ogre::Radian(Ogre::Degree(yawFactor)));
		Game::getPtr()->getGraphicsManager()->camPitch->pitch(Ogre::Radian(Ogre::Degree(pitchFactor)));

		cashText->text("%5Cash: $"+Ogre::StringConverter::toString(currentCash));
		depthText->text("%5Depth: "+Ogre::StringConverter::toString(Game::getPtr()->getGraphicsManager()->mCamera->getDerivedPosition().y/-2.f+1000.f,4)+"m");
		speedText->text("%5Speed: "+Ogre::StringConverter::toString(mPlayer->getVelocity().length()*0.5f,2)+"m/s");// in knots, 'cuz we're underwater ;)
		fpsText->text("%5Weight: "+Ogre::StringConverter::toString(currentWeight)+" / "+Ogre::StringConverter::toString(maxWeight)+"kg");// in knots, 'cuz we're underwater ;)

		std::pair<unsigned short,unsigned short> c = getCoords(Game::getPtr()->getGraphicsManager()->mCamera->getDerivedPosition());

		if(mLastCoord.first!=c.first||mLastCoord.second!=c.second)
			pingNeighbors(c);
		mLastCoord = c;


		RaycastReport rr = Game::getPtr()->getPhysicsManager()->raycastSimple(mPlayer->getPosition()+Game::getPtr()->getGraphicsManager()->mCamera->getDerivedDirection()*3.1f,Game::getPtr()->getGraphicsManager()->mCamera->getDerivedDirection(),5);
		
		//fpsText->text("...");

		pickText->text("");
		pickWeight->text("");
		pickValue->text("");
		pickContext->text("");
		
		if(rr.hit)
		{
			if(rr.userData)
			{
				GameObject* d = reinterpret_cast<GameObject*>(rr.userData);
				if(d->getType()=="Treasure")
				{
					Treasure* t = dynamic_cast<Treasure*>(d);

					pickText->text(t->displayName);
					pickWeight->text("Weight: "+Ogre::StringConverter::toString(t->weight)+"kg");
					pickValue->text("Est. Value: $"+Ogre::StringConverter::toString(t->value));
					if(currentWeight+t->weight<=maxWeight)
					{
						pickContext->text("Left click to grab");
						pickContext->colour(Ogre::ColourValue(0.3,0.9,0.45));
					}
					else
					{
						pickContext->text("Too Heavy!");
						pickContext->colour(Ogre::ColourValue(0.9,0.4,0.3));
						fpsText->text("%4"+fpsText->text().substr(2));
					}
					//fpsText->text("Object: "+t->displayName);
					if(isMouseDown(OIS::MB_Left)&&t->weight+currentWeight<=maxWeight)//isKeyDown(OIS::KC_SPACE))
					{
						t->pickedUp();
						inventory.push_back(t);
						currentWeight+=t->weight;
						Game::getPtr()->mAudio->play2D("media/audio/pick.wav");
					}
				}
			}
		}

		if(Ogre::Math::Sqrt(mPlayer->getPosition().x*mPlayer->getPosition().x+mPlayer->getPosition().z*mPlayer->getPosition().z)<=11.f)
		{
			cashOut->text("Press SPACE to recharge and sell items");
			cashOut2->text("Or ENTER to buy stuff");
		}
		else
		{
			cashOut->text("");
			cashOut2->text("");
		}


		//Ogre::Vector3 pos = Game::getPtr()->getGraphicsManager()->mCamera->getDerivedPosition();
		//Real dist = pos.distance(lastPos);
		//lastPos = pos;

		//Real speed = 10.f/1000.f/(1.f/3600.f)*1.852f;// km/hr *
		//speedText->text("%5         Speed: "+Ogre::StringConverter::toString((speed+lastSpeed)/2)+"kn");
		//lastSpeed = speed;

		// knot = 1.852 km/h

		energy-=(currentSpeed/(maxSpeed-(maxSpeed/3.65)*(1.f*currentWeight/maxWeight)))/175.f*delta*energyRate;
		energy-=delta*0.008*lightsOn*energyRate+delta*0.001*engineOn*energyRate;
		if(energy<=0)
		{
			DEATH->text("OUT OF ENERGY - PRESS R TO RESPAWN");
			energy = 0.0;
			engineOn = false;
			mLight->setVisible(engineOn&&lightsOn);
			Game::getPtr()->getGraphicsManager()->lt->setVisible(engineOn&&lightsOn);
			Game::getPtr()->getGraphicsManager()->lt3->setVisible(engineOn&&lightsOn);
			if(engineOn)
			{
				mPlayer->setAngularFactor(Ogre::Vector3(1,1,1));

				if(lightsOn)
					lightText->text("%5Lights: On (E)");
				else
					lightText->text("%5Lights: Off (E)");
				engineText->text("%5Engine: On (Q)");
			}
			else
			{
				//mPlayer->setAngularFactor(Ogre::Vector3(0,0,0));

				engineText->text("%5Engine: Off (Q)");
				lightText->text("%5Lights: Off (E)");
			}
		}
		energyBar->width(energy*512);

	}
	else if(cashingOut)
	{
		Game::getPtr()->getGraphicsManager()->getLayer(0)->hide();
		Game::getPtr()->getGraphicsManager()->getLayer(1)->show();
		Game::getPtr()->getGraphicsManager()->getLayer(2)->hide();
		Game::getPtr()->getGraphicsManager()->getLayer(3)->show();

		cashTimer-=delta;
		if(cashTimer<0.f&&cashingStill)
		{
			if(inventory.size()>0)
			{
				pushCash();
				cashTimer = 0.2f;
				if(inventory.empty())
					cashingStill = false;
			}
			else
			{
				cashingStill = false;
			}
		}
	}
	else if(buying)
	{
		Game::getPtr()->getGraphicsManager()->getLayer(0)->hide();
		Game::getPtr()->getGraphicsManager()->getLayer(3)->hide();
		Game::getPtr()->getGraphicsManager()->getLayer(2)->show();
		Game::getPtr()->getGraphicsManager()->getLayer(1)->show();

		cashBuy->text("%6Cash:  $"+Ogre::StringConverter::toString(currentCash));
	}

	return true;
}

std::pair<unsigned short,unsigned short> TestState::getCoords(Ogre::Vector3 pos)
{
	// subtract 40, since the center is, well, centered...
	float xpos = pos.x-40.f;
	float zpos = pos.z-40.f;

	// squares are 80x80
	xpos/=80.f;
	zpos/=80.f;

	// the center is (16384,16384)
	unsigned short realx = floor(xpos)+16384;
	unsigned short realz = floor(zpos)+16384;

	return std::pair<unsigned short,unsigned short>(realx,realz);
}

void TestState::pingNeighbors(std::pair<unsigned short,unsigned short> coords)
{
	for(uint i=0;i<3;++i)
	{
		for(uint j=0;j<3;++j)
		{
			unsigned short x = coords.first+i;
			unsigned short z = coords.second+j;
			if(mGrids.find(x)!=mGrids.end()&&mGrids[x].find(z)!=mGrids[x].end())
			{
				// it exists, good for it
			}
			else
			{
				//make it
				mGrids[x][z] = new Grid("Grid_"+Ogre::StringConverter::toString(x)+"_"+Ogre::StringConverter::toString(z),"grids",x,z);
				//std::cout<<"creating grid square at: "<<x<<" "<<z<<"\n";
			}
		}
	}
}


bool TestState::keyReleased(const OIS::KeyEvent &evt) 
{
	if(!cashingOut&&!buying)
	{
		if(evt.key==OIS::KC_E)
		{
			Game::getPtr()->mAudio->play2D("media/audio/switch.wav");
			lightsOn = !lightsOn;
			mLight->setVisible(lightsOn);
			Game::getPtr()->getGraphicsManager()->lt->setVisible(lightsOn);
			Game::getPtr()->getGraphicsManager()->lt3->setVisible(lightsOn);
			if(lightsOn)
				lightText->text("%5Lights: On (E)");
			else
				lightText->text("%5Lights: Off (E)");
		}
		else if(evt.key==OIS::KC_Q&&energy>0.01)
		{
			Game::getPtr()->mAudio->play2D("media/audio/switch_e.wav");

			engineOn = !engineOn;
			mLight->setVisible(engineOn&&lightsOn);
			Game::getPtr()->getGraphicsManager()->lt->setVisible(engineOn&&lightsOn);
			Game::getPtr()->getGraphicsManager()->lt3->setVisible(engineOn&&lightsOn);
			if(engineOn)
			{
				idle->setVolume(0.65f);
				motor->setVolume(0.f);
				mPlayer->setAngularFactor(Ogre::Vector3(1,1,1));

				if(lightsOn)
					lightText->text("%5Lights: On (E)");
				else
					lightText->text("%5Lights: Off (E)");
				engineText->text("%5Engine: On (Q)");
			}
			else
			{
				idle->setVolume(0.f);
				motor->setVolume(0.f);
				//mPlayer->setAngularFactor(Ogre::Vector3(0,0,0));

				engineText->text("%5Engine: Off (Q)");
				lightText->text("%5Lights: Off (E)");
			}
		}
		else if(evt.key==OIS::KC_F&&beacons>0)
		{
			--beacons;
			beaconText->text("%5Beacons: "+Ogre::StringConverter::toString(beacons)+" (F)");
			RaycastReport rr = Game::getPtr()->getPhysicsManager()->raycastSimple(mPlayer->getPosition()+Game::getPtr()->getGraphicsManager()->mCamera->getDerivedDirection()*3.1f,Game::getPtr()->getGraphicsManager()->mCamera->getDerivedDirection(),5);
			
			if(rr.hit)
			{
				Beacon* b = new Beacon("Beacon"+nrBeacons,"Default",rr.hitP+Ogre::Vector3(0,0.2f,0),0);
			}
			else
			{
				Ogre::Vector3 bpos = mPlayer->getPosition();
				Ogre::Vector3 dr = Game::getPtr()->getGraphicsManager()->mCamera->getDerivedDirection()*3.1f;
				Ogre::Vector3 beaconPos = bpos + dr + Game::getPtr()->getGraphicsManager()->mCamera->getDerivedDirection()*5.f;
				std::pair<unsigned short,unsigned short> coords = getCoords(bpos);
				Beacon* b = new Beacon("Beacon"+nrBeacons,"Default",beaconPos,mGrids[coords.first][coords.second]);
			}


			++nrBeacons;
		}
		else if(energy<0.01f&&evt.key==OIS::KC_R)
		{
			mPlayer->setPosition(Ogre::Vector3(0,-5,22));
			energy = 1;
			DEATH->text("");
			inventory.clear();
			currentWeight = 0;
			engineOn = true;
			lightsOn = true;

			mLight->setVisible(engineOn&&lightsOn);
			Game::getPtr()->getGraphicsManager()->lt->setVisible(engineOn&&lightsOn);
			Game::getPtr()->getGraphicsManager()->lt3->setVisible(engineOn&&lightsOn);
			if(engineOn)
			{
				idle->setVolume(0.65f);
				motor->setVolume(0.f);
				mPlayer->setAngularFactor(Ogre::Vector3(1,1,1));

				if(lightsOn)
					lightText->text("%5Lights: On (E)");
				else
					lightText->text("%5Lights: Off (E)");
				engineText->text("%5Engine: On (Q)");
			}
			else
			{
				idle->setVolume(0.f);
				motor->setVolume(0.f);
				//mPlayer->setAngularFactor(Ogre::Vector3(0,0,0));

				engineText->text("%5Engine: Off (Q)");
				lightText->text("%5Lights: Off (E)");
			}

		}
		else if(evt.key==OIS::KC_SPACE&&Ogre::Math::Sqrt(mPlayer->getPosition().x*mPlayer->getPosition().x+mPlayer->getPosition().z*mPlayer->getPosition().z)<=11.f)
		{
			cashingOut = true;
			cashingStill = true;
			cashTimer = 0.2f;
			Game::getPtr()->setTimeSpeed(0.f);
			// cash out!
			energy = 1.f;
			cashValCI->text("Net Balance: "+Ogre::StringConverter::toString(currentCash)+".00");
			prof->text("Profit: 0.00");
			profit = 0;
			for(uint i=0;i<10;++i)
			{
				names[i]->text(".");
				values[i]->text(".");
			}
		}
		else if(evt.key==OIS::KC_RETURN&&Ogre::Math::Sqrt(mPlayer->getPosition().x*mPlayer->getPosition().x+mPlayer->getPosition().z*mPlayer->getPosition().z)<=11.f)
		{
			buying = true;
			Game::getPtr()->setTimeSpeed(0.f);
			energy = 1.f;
			selection = 0;
			for(uint i=0;i<mOptions.size();++i)
			{
				mOptions[i]->updateText(0);
			}
			mOptions[0]->updateText(1);
		}
		else if(evt.key==OIS::KC_ESCAPE)
		{
			Game::getPtr()->mShutdown = true;
		}
	}
	else if(cashingOut)
	{
		if((evt.key==OIS::KC_SPACE||evt.key==OIS::KC_ESCAPE||evt.key==OIS::KC_RETURN)&&!cashingStill)
		{
			cashingOut = false;
			Game::getPtr()->setTimeSpeed(1.f);
		}
	}
	else if(buying)
	{
		if(evt.key==OIS::KC_SPACE||evt.key==OIS::KC_ESCAPE)
		{
			buying = false;
			Game::getPtr()->setTimeSpeed(1.f);
			maxSpeed = 7.25+mOptions[0]->purchaseLevel*2.f;
			speedRate = 2.38+mOptions[0]->purchaseLevel*0.75f;
			maxWeight = 250+mOptions[1]->purchaseLevel*250;
			energyRate = 1.f-mOptions[2]->purchaseLevel*0.175f;
			Game::getPtr()->getGraphicsManager()->lt3->setAttenuation(42+mOptions[3]->purchaseLevel*11,0,0,0);
			Game::getPtr()->getGraphicsManager()->lt3->setSpotlightInnerAngle(Ogre::Degree(20+mOptions[3]->purchaseLevel*8));
			Game::getPtr()->getGraphicsManager()->lt3->setSpotlightOuterAngle(Ogre::Degree(28+mOptions[3]->purchaseLevel*8));
			mLight->getBillboard(0)->setDimensions(1.f+mOptions[3]->purchaseLevel*0.3f,1.f+mOptions[3]->purchaseLevel*0.3f);
		}
		else if(evt.key==OIS::KC_DOWN)
		{
			Game::getPtr()->mAudio->play2D("media/audio/menu.wav");
			mOptions[selection]->updateText(0);
			++selection;
			if(selection>=mOptions.size())
				selection = 0;
			mOptions[selection]->updateText(1);
		}
		else if(evt.key==OIS::KC_UP)
		{
			Game::getPtr()->mAudio->play2D("media/audio/menu.wav");
			mOptions[selection]->updateText(0);
			--selection;
			if(selection<0)
				selection = mOptions.size()-1;
			mOptions[selection]->updateText(1);
		}
		else if(evt.key==OIS::KC_RETURN)
		{
			if(currentCash>=mOptions[selection]->getPrice())
			{
				// SFX HERE
				currentCash-=mOptions[selection]->getPrice();
				mOptions[selection]->buy();
				mOptions[selection]->updateText(1);
				if(mOptions[selection]->nam=="5x Beacons")
				{
					beacons+=5;
					beaconBuy->text("Beacons: "+Ogre::StringConverter::toString(beacons));
					beaconText->text("%5Beacons: "+Ogre::StringConverter::toString(beacons)+" (F):");
					Game::getPtr()->mAudio->play2D("media/audio/b.wav");
				}
				else
				{
					Game::getPtr()->mAudio->play2D("media/audio/upgrade.wav");
				}
			}
		}
	}
	mKeyStates[evt.key] = false;
	return true;
}

void TestState::pushCash()
{
	if(inventory.size()>0)
	{
		Game::getPtr()->mAudio->play2D("media/audio/coin01.wav");
		int value = inventory[0]->value-(1+rand()%inventory[0]->minusValue)+(1+rand()%inventory[0]->bonusValue);
		currentWeight-=inventory[0]->weight;
		currentCash+=value;
		profit+=value;
		String nm = inventory[0]->displayName;
		inventory.erase(inventory.begin());
		cashValCI->text("Net Balance: "+Ogre::StringConverter::toString(currentCash)+".00");
		prof->text("Profit: "+Ogre::StringConverter::toString(profit)+".00");

		String tempNm = nm;
		String tempNm2 = nm;
		String tempVal = "$"+Ogre::StringConverter::toString(value)+".00";
		String tempVal2 = "$"+Ogre::StringConverter::toString(value)+".00";

		for(uint i=0;i<10;++i)
		{
			tempNm2 = names[i]->text();
			tempVal2 = values[i]->text();
			names[i]->text(tempNm);
			values[i]->text(tempVal);
			tempNm = tempNm2;
			tempVal = tempVal2;
		}
	}
}