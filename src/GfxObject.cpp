#include "stdafx.h"
#include "GfxObject.h"

GraphicsObject::GraphicsObject(Ogre::SceneNode* nd,Ogre::Entity* ent)
{
	node = nd;
	entity = ent;
}

GraphicsObject::~GraphicsObject()
{
	node->detachAllObjects();
	node->getCreator()->destroyEntity(entity);
	node->getCreator()->destroySceneNode(node);
}

void GraphicsObject::setPosition(Ogre::Vector3 pos)
{
	node->setPosition(pos);
}

Ogre::Vector3 GraphicsObject::getPosition()
{
	return node->getPosition();
}