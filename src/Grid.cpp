#include "stdafx.h"

#include "Grid.h"
#include "Game.h"

#include "TreasureCache.h"
#include "PhysicsManager.h"

float randFloat(float low, float high)
{
	float temp;

	if (low > high)
	{
		temp = low;
		low = high;
		high = temp;
	}

	temp = (rand() / (static_cast<float>(RAND_MAX) + 1.0f))
	* (high - low) + low;
	return temp;
};

Grid::Grid(String name,String bucket,unsigned short x,unsigned short y,uint type)
	:GameObject(name,bucket)
{
	int randy = rand()%10;
	if(type!=50000)
		randy = type;

	//randy = 3;

	String mesh = "BasicTerrain";
	if(randy==0)
		mesh = "BasicTerrain";
	else
	{
		mesh = "Terrain";
		if(randy<10)
			mesh+="0";
		mesh+=Ogre::StringConverter::toString(randy);
	}

	gfx = Game::getPtr()->getGraphicsManager()->createGraphicsObject(mesh+".mesh");
	gfx->setPosition(Ogre::Vector3((x-16384)*80.f,-10,(y-16384)*80.f));
	phys = Game::getPtr()->getPhysicsManager()->createStaticTrimesh(mesh+"P.mesh",gfx->getPosition());
	phys->setUserData(this);


	int lm = rand()%(4+static_cast<int>(gfx->getPosition().length()/240.f))+3;
	if(x==16384&&y==16384)
		lm = 0;
	for(uint i=0;i<lm;++i)
	{
		placeTreasure();
	}


	if(randy==3)
	{
		placeTreasure(Ogre::Vector3(3.836,-15.96,-0.694));
	}
}

Grid::~Grid()
{

}

void Grid::update(Real delta)
{
	Ogre::Vector3 playerPos = Ogre::Root::getSingletonPtr()->
		getSceneManager("mSceneManager")->getCamera("mCamera")->getDerivedPosition();
	if(gfx->node->getPosition().distance(playerPos)>160.f)
	{
		gfx->node->setVisible(false);
	}
	else
	{
		gfx->node->setVisible(true);
	}
}

void Grid::placeTreasure()
{
	uint rare = gfx->getPosition().length()/15.f;

	Ogre::Vector3 pos = gfx->getPosition()+Ogre::Vector3(randFloat(-30.f,30.f),0,randFloat(-30.f,30.f));

	RaycastReport rr = Game::getPtr()->getPhysicsManager()->raycastSimple(pos+Ogre::Vector3(0,100,0),Ogre::Vector3(0,-1,0));
	if(rr.hit)
	{
		pos = rr.hitP-gfx->getPosition();// level with terrain, maybe eventually use normal to orient it...
		caches.push_back(new TreasureCache(mName+"_Cache_"+Ogre::StringConverter::toString(caches.size()),"caches",pos,this,rare));
		caches[caches.size()-1]->croot->setOrientation(Ogre::Vector3(0,1,0).getRotationTo(rr.normal));
		for(uint i=0;i<caches[caches.size()-1]->phys.size();++i)
		{
			caches[caches.size()-1]->phys[i]->setOrientation(Ogre::Vector3(0,1,0).getRotationTo(rr.normal));
		}
	}
	else
	{
		caches.push_back(new TreasureCache(mName+"_Cache_"+Ogre::StringConverter::toString(caches.size()),"caches",pos,this,rare));
	}
}


void Grid::placeTreasure(Ogre::Vector3 posi)
{
	uint rare = gfx->getPosition().length()/32.f;
	Ogre::Vector3 pos = posi;
	caches.push_back(new TreasureCache(mName+"_Cache_"+Ogre::StringConverter::toString(caches.size()),"caches",pos,this,rare));
}
