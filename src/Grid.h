#ifndef GRID_H
#define GRID_H

#include "stdafx.h"
#include "GfxObject.h"
#include "PhysicsObject.h"

#include "GameObject.h"

class TreasureCache;

class Grid : public GameObject
{
public:
	Grid(String name,String bucket,unsigned short x,unsigned short y,uint type=50000);
	~Grid();
	void update(Real delta);

	void placeTreasure();
	void placeTreasure(Ogre::Vector3 pos);

	GraphicsObject* gfx;
	PhysicsObject* phys;
	std::vector<TreasureCache*> caches;
};

#endif