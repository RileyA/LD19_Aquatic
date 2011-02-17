#ifndef GFXOBJ_H
#define GFXOBJ_H

#include "stdafx.h"

class GraphicsObject
{
public:

	GraphicsObject(Ogre::SceneNode* nd,Ogre::Entity* ent);
	~GraphicsObject();

	void setPosition(Ogre::Vector3);
	Ogre::Vector3 getPosition();

	Ogre::SceneNode* node;
	Ogre::Entity* entity;

};

#endif