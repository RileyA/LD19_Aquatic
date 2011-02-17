#ifndef BUCKET_H
#define BUCKET_H

#include "stdafx.h"
#include "GameObject.h"

class Bucket
{
public:

	Bucket(String name)
	{
		mName = name;
	}

	~Bucket()
	{
		std::map<String,GameObject*>::iterator i = mObjects.begin();
		for(i;i!=mObjects.end();++i)
			delete i->second;
	}

	void update(Real delta)
	{
		std::map<String,GameObject*>::iterator i = mObjects.begin();
		for(i;i!=mObjects.end();++i)
			i->second->update(delta);
	}

	void pushObject(GameObject* obj,String name)
	{
		mObjects[name] = obj;
	}

	GameObject* getObject(String name)
	{
		if(mObjects.find(name)==mObjects.end())
			return 0;
		return mObjects[name];
	}

	String mName;
	std::map<String,GameObject*> mObjects;
};

#endif