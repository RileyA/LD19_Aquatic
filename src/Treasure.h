#ifndef TREASURE_H
#define TREASURE_H

#include "stdafx.h"
#include "GfxObject.h"
#include "PhysicsObject.h"

#include "GameObject.h"

class Treasure : public GameObject
{
public:
	Treasure(String name,String bucket, String mesh,Ogre::Vector3 pos);
	~Treasure();
	void update(Real delta);

	void pickedUp();

	GraphicsObject* gfx;
	PhysicsObject* phys;
	int value;// $$$
	int weight;// kg
	int bonusValue;
	int minusValue;
	String displayName;

	bool picked;
};

#endif