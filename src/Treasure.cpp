#include "stdafx.h"
#include "Treasure.h"

Treasure::Treasure(String name,String bucket, String mesh,Ogre::Vector3 pos)
	:GameObject(name,bucket)
{
	mType = "Treasure";
	picked = false;
	gfx = Game::getPtr()->getGraphicsManager()->createGraphicsObject(mesh,false);
	gfx->setPosition(pos);
	phys = Game::getPtr()->getPhysicsManager()->createStaticTrimesh(mesh,pos);
	//phys->setKinematic(true);//sure....
	//phys->setMass(0.f);
	//phys->setAngularFactor(Ogre::Vector3(0,0,0));
	//phys->setLinearFactor(Ogre::Vector3(0,0,0));
	phys->setUserData(this);
	value = 500;
	weight = 50;
	minusValue = 10;
	if(mesh=="Chest01.mesh")
	{
		value = 1000;
		weight = 50;
		minusValue = 400;
		bonusValue = 550;// up to +500
		displayName = "Old Chest";
	}
	else if(mesh=="Chest02.mesh")
	{
		value = 1500;
		weight = 65;
		minusValue = 500;
		bonusValue = 1500;// up to +500
		displayName = "Ancient Chest";
	}
	else if(mesh=="Crate01.mesh")
	{
		value = 250;
		weight = 40;
		minusValue = 100;
		bonusValue = 500;// up to +500
		displayName = "Wooden Crate";
	}
	else if(mesh=="crate02.mesh")
	{
		value = 800;
		weight = 100;
		bonusValue = 1000;// up to +500
		minusValue = 300;
		displayName = "Metal Crate";
	}
	else if(mesh=="chembarrel.mesh")
	{
		value = 200;
		weight = 75;
		minusValue = 150;
		bonusValue = 800;// up to +500
		displayName = "Chemical Barrels";	
	}
	else if(mesh=="wine01.mesh")
	{
		value = 500;
		weight = 150;
		minusValue = 350;
		bonusValue = 600;// up to +500
		displayName = "Wine Barrels";	
	}
	else if(mesh=="goldbars.mesh")
	{
		value = 5000;
		weight = 100;
		bonusValue = 2000;// up to +500
		minusValue = 500;
		displayName = "Gold bars";	
	}
	else if(mesh=="Bomb.mesh")
	{
		value = 10000;
		weight = 250;
		bonusValue = 2000;// up to +500
		minusValue = 3000;
		displayName = "WWII Bombs";	
	}
}

Treasure::~Treasure()
{
	pickedUp();
}

void Treasure::update(Real delta)
{
	if(!picked)
	{
		// set invisible at distance... TODO
		phys->setPosition(gfx->node->_getDerivedPosition());
		phys->setOrientation(gfx->node->_getDerivedOrientation());
	}
}

void Treasure::pickedUp()
{
	if(!picked)
	{
		Game::getPtr()->getGraphicsManager()->destroyGfxObj(gfx);
		phys->_kill();
		picked = true;
	}
}