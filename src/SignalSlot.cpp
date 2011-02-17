#include "stdafx.h"
#include "SignalSlot.h"

//---------------------------------------------------------------------------
//-------------------| SIGNAL |----------------------------------------------
//---------------------------------------------------------------------------
Signal::Signal(){};
//---------------------------------------------------------------------------

Signal::~Signal()
{
	removeAllListeners();
}
//---------------------------------------------------------------------------

void Signal::fire(Message* msg)
{
	for(unsigned int i=0;i<mListeners.size();++i)
	{
		mListeners[i]->slot(msg);
	}
}
//---------------------------------------------------------------------------

void Signal::addListener(Slot* slot)
{
	mListeners.push_back(slot);
}
//---------------------------------------------------------------------------

void Signal::removeListener(Slot* slot)
{
	for(unsigned int i=0;i<mListeners.size();++i)
	{
		if(mListeners[i]==slot)
		{
			mListeners[i]->stopListeningTo(this);
			mListeners.erase(mListeners.begin()+i);
			break;
		}
	}
}
//---------------------------------------------------------------------------

void Signal::removeAllListeners()
{
	for(unsigned int i=0;i<mListeners.size();++i)
	{
		mListeners[i]->stopListeningTo(this);
	}
	mListeners.clear();
}
//---------------------------------------------------------------------------

void Signal::operator += (Slot* slot)
{
	mListeners.push_back(slot);
}
//---------------------------------------------------------------------------

void Signal::operator -= (Slot* slot)
{
	for(unsigned int i=0;i<mListeners.size();++i)
	{
		if(mListeners[i]==slot)
		{
			mListeners[i]->stopListeningTo(this);
			mListeners.erase(mListeners.begin()+i);
			break;
		}
	}
}
//---------------------------------------------------------------------------
//--------------------| SLOT |-----------------------------------------------
//---------------------------------------------------------------------------
Slot::Slot(Delegate _slot)
{
	slot = _slot;
}
//---------------------------------------------------------------------------

Slot::~Slot()
{
	stopAllListening();
}
//---------------------------------------------------------------------------

void Slot::listenTo(Signal* signal)
{
	mSignals.push_back(signal);
}
//---------------------------------------------------------------------------

void Slot::stopListeningTo(Signal* signal)
{
	for(unsigned int i=0;i<mSignals.size();++i)
	{
		if(mSignals[i]==signal)
		{
			mSignals[i]->removeListener(this);
			mSignals.erase(mSignals.begin()+i);
			break;
		}
	}
}
//---------------------------------------------------------------------------

void Slot::stopAllListening()
{
	for(unsigned int i=0;i<mSignals.size();++i)
	{
		mSignals[i]->removeListener(this);
	}
	mSignals.clear();
}
//---------------------------------------------------------------------------

void Slot::operator += (Signal* signal)
{
	mSignals.push_back(signal);
}
//---------------------------------------------------------------------------

void Slot::operator -= (Signal* signal)
{
	for(unsigned int i=0;i<mSignals.size();++i)
	{
		if(mSignals[i]==signal)
		{
			mSignals[i]->removeListener(this);
			mSignals.erase(mSignals.begin()+i);
			break;
		}
	}
}
//---------------------------------------------------------------------------