#include "stdafx.h"
#include "TreasureCache.h"
#include "Treasure.h"
#include "Grid.h"

TreasureCache::TreasureCache(String name,String bucket, Ogre::Vector3 pos, Grid* parent,uint rarity)
	:GameObject(name,bucket)
{
	mType = "TreasureCache";
	par = parent;
	if(rarity>29)
		rarity = 34;
	uint tp = rand()%(20+rarity);
	croot = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->createSceneNode();
	parent->gfx->node->addChild(croot);
	croot->setPosition(pos);

	// add to the top...
	if(tp>51)
	{
		gfx.push_back(Game::getPtr()->getGraphicsManager()->createGraphicsObject("Wreck02.mesh",false));
		phys.push_back(Game::getPtr()->getPhysicsManager()->createStaticTrimesh("Wreck02.mesh",croot->_getDerivedPosition()));
		croot->addChild(gfx[0]->node);
		addTreasure("Bomb.mesh",Ogre::Vector3(0,0,0));
	}
	else if(tp>48)
	{
		addTreasure("goldbars.mesh",Ogre::Vector3(0,0,0));
	}
	else if(tp>42)
	{
		addTreasure("Chest02.mesh",Ogre::Vector3(0,0,0));
	}
	else if(tp>35)
	{
		addTreasure("crate02.mesh",Ogre::Vector3(0,0,0));
	}
	else if(tp>30)
	{
		addTreasure("wine01.mesh",Ogre::Vector3(0,0,0));
	}
	else if(tp>25)
	{
		gfx.push_back(Game::getPtr()->getGraphicsManager()->createGraphicsObject("Wreck01.mesh",false));
		phys.push_back(Game::getPtr()->getPhysicsManager()->createStaticTrimesh("Wreck01.mesh",croot->_getDerivedPosition()));
		croot->addChild(gfx[0]->node);
		if((rand()%100>10))
		addTreasure("Chest01.mesh",Ogre::Vector3(-0.277,0.552,0.839),Ogre::Vector3(0,-257.72,0));
		if(rand()%2==0)
			addTreasure("Chest01.mesh",Ogre::Vector3(0.55,0.560,-1.714),Ogre::Vector3(32.552,-257.72,0));

	}
	else if(tp>19)
	{
		addTreasure("Chest01.mesh",Ogre::Vector3(0,0,0));
	}
	else if(tp>10)
	{
		addTreasure("chembarrel.mesh",Ogre::Vector3(0,0,0));
	}
	else
	{
		addTreasure("Crate01.mesh",Ogre::Vector3(0,0,0));
	}
}

TreasureCache::~TreasureCache()
{
	
}

void TreasureCache::update(Real delta)
{
	// visibility...
	for(uint i=0;i<treasures.size();--i)
	{
		if(treasures[i]->picked)
		{
			treasures.erase(treasures.begin()+i);
			--i;
			if(i<0)
				i=0;
		}
	}

}

void TreasureCache::addTreasure(String mesh,Ogre::Vector3 loc,Ogre::Vector3 rot)
{
	treasures.push_back(new Treasure(mName+"_Treasure_"+Ogre::StringConverter::toString(treasures.size()),"Default",mesh,loc));
	croot->addChild(treasures[treasures.size()-1]->gfx->node);

	treasures[treasures.size()-1]->gfx->node->yaw(Ogre::Degree(rot.y));
	treasures[treasures.size()-1]->gfx->node->pitch(Ogre::Degree(rot.x));
	treasures[treasures.size()-1]->gfx->node->roll(Ogre::Degree(rot.z));

	//treasures[treasures.size()-1]->phys->setPosition(treasures[treasures.size()-1]->gfx->node->getPosition()+croot->getPosition()+par->gfx->node->getPosition());
	//treasures[treasures.size()-1]->phys->setOrientation(treasures[treasures.size()-1]->gfx->node->getOrientation()*croot->_getDerivedOrientation());
}
