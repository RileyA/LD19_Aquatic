#include "stdafx.h"

#include "Beacon.h"
#include "Grid.h"

#include "PhysicsManager.h"

Beacon::Beacon(String name,String bucket,Ogre::Vector3 pos,Grid* parent)
	:GameObject(name,bucket)
{
	gfx = Game::getPtr()->getGraphicsManager()->createGraphicsObject("Beacon.mesh",false);
	par = parent;
	endPos = pos;
	//par->gfx->node->addChild(gfx->node);
	Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->getRootSceneNode()->addChild(gfx->node);
	RaycastReport rr = Game::getPtr()->getPhysicsManager()->raycastSimple(pos,Ogre::Vector3(0,-1,0),100000,COLLISION_GROUP_15,COLLISION_GROUP_15);
	if(rr.hit)
	{
		gfx->setPosition(rr.hitP);
		endPos = rr.hitP;
		gfx->node->setOrientation(Ogre::Vector3(0,1,0).getRotationTo(rr.normal));
	}

	gfx->setPosition(pos);
	
	light = Ogre::Root::getSingletonPtr()->getSceneManager("mSceneManager")->createBillboardSet(1);
	light->setMaterialName("LightB");
	light->setDefaultDimensions(0.8f,0.8f);
	gfx->node->attachObject(light);
	light->createBillboard(Ogre::Vector3(0,0.6f,0));
}


Beacon::~Beacon()
{

}

void Beacon::update(Real delta)
{
	//light->setVisible(true);
	if(endPos!=gfx->getPosition())
	{
		Real len = delta*10.0;
		if(len>endPos.distance(gfx->getPosition()))
			gfx->setPosition(endPos);
		else
			gfx->setPosition(gfx->getPosition()-Ogre::Vector3(0,1,0)*len);
	}
}