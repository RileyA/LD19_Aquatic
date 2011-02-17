#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "stdafx.h"

#include "GfxObject.h"


class GraphicsManager
{
public:

    GraphicsManager();
    ~GraphicsManager();

    void init(uint resX,uint resY,bool vsync,bool fullscreen);
    void deinit();

    void renderFrame();

    void update();

    Ogre::Root* getRoot(){return mRoot;}

	Gorilla::Layer* getLayer(uint index);

	Ogre::RenderWindow* getWindow(){return mWindow;}

	GraphicsObject* createGraphicsObject(String mesh,bool root=true);

	void endState();

	void destroyGfxObj(GraphicsObject* obj);

	void setFog(float _near, float _far,float exp);

	Ogre::SceneNode* camRoll;
	Ogre::SceneNode* camYaw;
	Ogre::SceneNode* camPitch;
	Ogre::SceneNode* camPos;
	Real pichLimit;
	Real pitch;
	bool cameraMotion;
    Ogre::Camera* mCamera;

	// these shouldn't be stored here, but they are so whatevs...
	Ogre::Light* lt;
	Ogre::Light* lt2;
	Ogre::Light* lt3;


private:

    Ogre::Root* mRoot;
    Ogre::SceneManager* mSmgr;
    Ogre::RenderWindow* mWindow;
    Ogre::Viewport* mViewport;

	Gorilla::Silverback* mGorilla;
	Gorilla::Screen* mScreen;
	Gorilla::Layer* mLayers[16];

    bool mInitialized;


	std::vector<GraphicsObject*> mObjects;

};


#endif
