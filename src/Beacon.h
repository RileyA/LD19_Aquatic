#ifndef BEACON_H
#define BEACON_H

#include "stdafx.h"
#include "GfxObject.h"
#include "PhysicsObject.h"

#include "GameObject.h"

class Grid;

class Beacon : public GameObject
{
public:
	Beacon(String name,String bucket,Ogre::Vector3 pos,Grid* parent);
	~Beacon();
	void update(Real delta);

	GraphicsObject* gfx;
	Ogre::BillboardSet* light;
	Grid* par;
	Ogre::Vector3 endPos;
};

#endif