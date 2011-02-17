#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "stdafx.h"
#include "Game.h"

class GameObject
{
public:

	virtual void update(Real delta) = 0;

	GameObject(String name,String bucket)
	{
		mName = name;
		Game::getPtr()->registerObject(this,name,bucket);
	}

	String getName()
	{
		return mName;
	}

	String getType()
	{
		return mType;
	}

	String mType;
	String mName;

};

#endif