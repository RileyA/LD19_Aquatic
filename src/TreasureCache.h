#ifndef TREASURECACHE_H
#define TREASURECACHE_H

#include "stdafx.h"
#include "GfxObject.h"
#include "PhysicsObject.h"

#include "GameObject.h"

class Treasure;
class Grid;

class TreasureCache : public GameObject
{
public:

	TreasureCache(String name,String bucket, Ogre::Vector3 pos, Grid* parent,uint rarity);
	~TreasureCache();
	void update(Real delta);

	void addTreasure(String mesh,Ogre::Vector3 loc,Ogre::Vector3 rot=Ogre::Vector3(0,0,0));

	std::vector<Treasure*> treasures;
	std::vector<GraphicsObject*> gfx;
	std::vector<PhysicsObject*> phys;

	Ogre::SceneNode* croot;
	Grid* par;

};

#endif