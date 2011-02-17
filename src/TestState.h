#ifndef TESTSTATE_H
#define TESTSTATE_H

#include "stdafx.h"
#include "State.h"

#include "Grid.h"

class Treasure;

struct BuyOption
{
	BuyOption(String n)
	{
		nam = n;
		purchaseLevel = 0;
		myInd = y;
		++y;
	}

	void createText()
	{
		price = Game::getPtr()->getGraphicsManager()->getLayer(2)->createMarkupText(24,150,100+60*myInd,"");
		name = Game::getPtr()->getGraphicsManager()->getLayer(2)->createMarkupText(24,325,100+60*myInd,nam);
		level = Game::getPtr()->getGraphicsManager()->getLayer(2)->createMarkupText(24,650,100+60*myInd,"");
		updateText();
	}

	void updateText(bool selected=false)
	{
		String pre = "";
		if(selected)
			pre = "%3";
		if(purchaseLevel<prices.size())
		{
			price->text(pre+"$"+Ogre::StringConverter::toString(prices[purchaseLevel]));
		}
		else
		{
			price->text(pre+"BOUGHT");
		}
		name->text(pre+nam);
		level->text("");
		for(uint i=0;i<prices.size()&&nam!="5x Beacons";++i)
		{
			if(i<purchaseLevel)
				level->text(level->text()+"%:yesup%  ");
			else
				level->text(level->text()+"%:noup%  ");
		}
	}

	void buy()
	{
		if(purchaseLevel<prices.size()&&nam!="5x Beacons")
			++purchaseLevel;
	}

	int getPrice()
	{
		if(purchaseLevel<prices.size())
			return prices[purchaseLevel];
		return 0;
	}

	String nam;
	std::vector<int> prices;
	int purchaseLevel;

	Gorilla::MarkupText* price;
	Gorilla::MarkupText* name;
	Gorilla::MarkupText* level;


	int myInd;

	static int y;

};

class TestState : public State
{
public:
	TestState();
	~TestState();

	void init();
	void deinit();
	bool update(Real delta);

	std::map<int,std::map<int,Grid*> > mGrids;// center is max ushort/2 * 2

	std::pair<unsigned short,unsigned short> getCoords(Ogre::Vector3 pos);

	void pingNeighbors(std::pair<unsigned short,unsigned short> coords);

	bool keyReleased(const OIS::KeyEvent &evt);
	irrklang::ISound* idle;
	irrklang::ISound* motor;


	double energy;

	Gorilla::Layer* hud;
	Gorilla::Rectangle* energyBar;
	Gorilla::MarkupText* fpsText;
	Gorilla::MarkupText* depthText;
	Gorilla::MarkupText* speedText;

	Gorilla::Caption* pickText;
	Gorilla::Caption* pickValue;
	Gorilla::Caption* pickWeight;
	Gorilla::Caption* pickContext;

	Gorilla::Caption* cashOut;
	Gorilla::Caption* cashOut2;

	Gorilla::MarkupText* cashText;

	Gorilla::MarkupText* engineText;
	Gorilla::MarkupText* lightText;
	Gorilla::MarkupText* beaconText;

	int beacons;

	Ogre::Vector3 lastPos;
	Real lastSpeed;

	PhysicsObject* mPlayer;

	Ogre::BillboardSet* mLandingLights;
	Ogre::BillboardSet* mLight;

	float maxSpeed;
	float minSpeed;
	float speedRate;
	float currentSpeed;

	int maxWeight;
	int currentWeight;

	std::pair<unsigned short,unsigned short> mLastCoord;

	std::vector<Treasure*> inventory;
	int nrBeacons;
	bool engineOn;
	bool lightsOn;

	int currentCash;

	// why did I even make a state machine???
	bool cashingOut;
	bool buying;

	// cashin stuff
	bool cashingStill;
	float cashTimer;
	int profit;
	std::vector<Gorilla::Caption*> names;
	std::vector<Gorilla::Caption*> values;
	Gorilla::MarkupText* cashValCI;
	Gorilla::MarkupText* prof;
	
	void pushCash();

	std::vector<BuyOption*> mOptions;
	int selection;
	Gorilla::MarkupText* cashBuy;
	Gorilla::MarkupText* beaconBuy;

	Gorilla::Caption* DEATH;

	double energyRate;

	float sensitivity;


};

#endif