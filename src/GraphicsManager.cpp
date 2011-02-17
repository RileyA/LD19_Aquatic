#include "stdafx.h"
#include "GraphicsManager.h"
#include "Game.h"

GraphicsManager::GraphicsManager()
    :mRoot(0),
    mSmgr(0),
    mWindow(0),
    mInitialized(false)
{
	for(uint i=0;i<16;++i)
		mLayers[i]=0;
}
//---------------------------------------------------------------------------

GraphicsManager::~GraphicsManager()
{
    deinit();
}
//---------------------------------------------------------------------------

void GraphicsManager::init(uint resX,uint resY,bool vsync,bool fullscreen)
{
    if(!mInitialized)
    {
        mRoot = new Ogre::Root("","");

		mRoot->loadPlugin("RenderSystem_Direct3D9");
		Ogre::RenderSystem* rs = mRoot->getRenderSystemByName(
            "Direct3D9 Rendering Subsystem");
		mRoot->setRenderSystem(rs);

		mRoot->initialise(false);

		Ogre::NameValuePairList miscP;

        if(vsync)
			miscP["vsync"] = "true";
		else
			miscP["vsync"] = "false";
		miscP["FSAA"] = "0";
		miscP["gamma"] = "false";
		miscP["border"] = "fixed";
		miscP["colourDepth"] = "32";
		miscP["monitorIndex"] = "0";

		mWindow = mRoot->createRenderWindow(
                "Aquatic - Ludum Dare 19 Entry - Copyright Riley Adams 2010",
                resX,resY,fullscreen,&miscP);

        mWindow->setActive(true);

        mSmgr = mRoot->createSceneManager(Ogre::ST_GENERIC,"mSceneManager");
        mCamera = mSmgr->createCamera("mCamera");
        mCamera->setPosition(0,0,0);
        mCamera->setDirection(0,0,-1);
        mCamera->setFarClipDistance(300);
        mCamera->setNearClipDistance(0.01f);
        mCamera->setFOVy(Ogre::Radian(Ogre::Degree(60)));

		mCamera->setPosition(0,0,-1.5f);

		camRoll = mSmgr->createSceneNode();
		camYaw = mSmgr->createSceneNode();
		camPitch = mSmgr->createSceneNode();
		camPos = mSmgr->createSceneNode();

		mSmgr->getRootSceneNode()->addChild(camPos);
		camPos->addChild(camYaw);
		camYaw->addChild(camPitch);
		camPitch->addChild(camRoll);
		camRoll->attachObject(mCamera);
		
		lt = mSmgr->createLight();
		lt2 = mSmgr->createLight();
		lt3 = mSmgr->createLight();
		lt2->setPosition(0,-2,0);
		lt->setAttenuation(15.f,0.f,0.2f,0.00007f);
		lt3->setAttenuation(42.f,0.f,0.001f,0.f);
		lt3->setType(Ogre::Light::LT_SPOTLIGHT);
		lt3->setDirection(0,0,-1);
		lt3->setSpotlightInnerAngle(Ogre::Degree(20.f));
		lt3->setSpotlightOuterAngle(Ogre::Degree(28.f));
		lt3->setSpotlightFalloff(0.8f);

		lt2->setAttenuation(30.f,0.f,0.14f,0.007f);
		lt2->setDiffuseColour(1.f,0.2f,0.2f);
		lt->setDiffuseColour(1.f,0.9f,0.675f);
		lt3->setDiffuseColour(1.f,0.9f,0.675f);

		lt->setPosition(0,0,-2);
		lt3->setPosition(0,0,-2);

		//lt->setVisible(false);
		camRoll->attachObject(lt);
		camRoll->attachObject(lt3);
		// for external use...
		pichLimit = 80.f;
		pitch = 0.f;
		cameraMotion = false;

        mViewport = mWindow->addViewport(mCamera);
		mViewport->setBackgroundColour(Ogre::ColourValue(44.f/455.f,90.f/455.f,93.f/455.f));
        mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth())
                                / Ogre::Real(mViewport->getActualHeight()));

        Ogre::ResourceGroupManager* resourceMgr = Ogre::ResourceGroupManager::getSingletonPtr();
        resourceMgr->addResourceLocation("","FileSystem","General");
        resourceMgr->addResourceLocation("media","FileSystem","General");
        resourceMgr->addResourceLocation("media/models","FileSystem","General");
        resourceMgr->addResourceLocation("media/materials","FileSystem","General");
        resourceMgr->addResourceLocation("media/textures","FileSystem","General");
        resourceMgr->addResourceLocation("media/audio","FileSystem","General");
		resourceMgr->addResourceLocation("media/gui","FileSystem","General");
        resourceMgr->addResourceLocation("script","FileSystem","General");
		resourceMgr->initialiseAllResourceGroups();

		Ogre::SceneNode* nd = mSmgr->createSceneNode();
		nd->attachObject(mSmgr->createEntity("Arms.mesh"));
		nd->setPosition(0,0,-1.5);
		//nd->pitch(Ogre::Degree(90));
		//nd->yaw(Ogre::Degree(180));
		camRoll->addChild(nd);

		mGorilla = new Gorilla::Silverback();
		mGorilla->loadAtlas("dejavu");
		mScreen = mGorilla->createScreen(mViewport, "dejavu");

		Game::getPtr()->log("Ogre initialized!");
        mInitialized = true;
    }
}
//---------------------------------------------------------------------------

void GraphicsManager::deinit()
{
    if(mInitialized)
    {
		endState();
		delete mGorilla;
        delete mRoot;
		mGorilla = 0;
        mRoot = 0;

		Game::getPtr()->log("Ogre shutdown!");
        mInitialized = false;
    }
}
//---------------------------------------------------------------------------

void GraphicsManager::renderFrame()
{
	Ogre::WindowEventUtilities::messagePump();
	mRoot->renderOneFrame();
}
//---------------------------------------------------------------------------

void GraphicsManager::update()
{
	renderFrame();
}
//---------------------------------------------------------------------------

Gorilla::Layer* GraphicsManager::getLayer(uint index)
{
	if(index<0||index>15)
		return 0;
	if(mLayers[index]==0)
		mLayers[index] = mScreen->createLayer(index);
	return mLayers[index];
}

GraphicsObject* GraphicsManager::createGraphicsObject(String mesh,bool root)
{
	Ogre::SceneNode* nd = mSmgr->createSceneNode();
	Ogre::Entity* ent = mSmgr->createEntity(mesh);
	nd->attachObject(ent);
	if(root)
		mSmgr->getRootSceneNode()->addChild(nd);
	mObjects.push_back(new GraphicsObject(nd,ent));
	return mObjects[mObjects.size()-1];
}

void GraphicsManager::endState()
{
	for(uint i=0;i<mObjects.size();++i)
	{
		delete mObjects[i];
	}
	mObjects.clear();
}

void GraphicsManager::setFog(float _near, float _far,float exp)
{
	mSmgr->setFog(mSmgr->getFogMode(),mSmgr->getFogColour(),exp,_near,_far);
}

void GraphicsManager::destroyGfxObj(GraphicsObject* obj)
{
	for(uint i=0;i<mObjects.size();++i)
	{
		if(obj==mObjects[i])
		{
			delete mObjects[i];
			mObjects.erase(mObjects.begin()+i);
			break;
		}
	}
}
