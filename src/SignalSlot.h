#ifndef SIGNALSLOT_H
#define SIGNALSLOT_H

#include "stdafx.h"

// signal/slot event stuff

//---------------------------------------------------------------------------

class Message
{
public:
	virtual ~Message(){};
};

typedef fastdelegate::FastDelegate1<Message*> Delegate;

//---------------------------------------------------------------------------

template<class T>
class MessageAny : public Message
{
public:
	MessageAny(T _data){data = _data;}
	T data;
};
//---------------------------------------------------------------------------

template<class T> MessageAny<T>& getMessageData(Message* msg)
{
	if(MessageAny<T>* message = dynamic_cast<MessageAny<T>* >(msg))
	{
		return message->data;
	}
	else
	{
		return 0;
	}
}
//---------------------------------------------------------------------------

struct Signal;// forward declare this...

// a slot that can recieve signals
struct Slot
{
public:

	Delegate slot;

	Slot(){}
	Slot(Delegate _slot);
	~Slot();

	void listenTo(Signal* signal);
	void stopListeningTo(Signal* signal);
	void stopAllListening();

	void operator += (Signal* signal);
	void operator -= (Signal* signal);

protected:

	// Signals this slot is subscribed to
	std::vector<Signal*> mSignals;
};
//---------------------------------------------------------------------------

// A "signal" that can be listened to by slots to make stuff communicate
struct Signal
{
public:

	Signal();
	~Signal();

	void fire(Message* msg);

	void addListener(Slot* slot);
	void removeListener(Slot* slot);
	void removeAllListeners();

	void operator += (Slot* slot);
	void operator -= (Slot* slot);

protected:

	std::vector<Slot*> mListeners;

};
//---------------------------------------------------------------------------

// manages signals/slots for an object
class EventHandler
{
public:

	EventHandler(){};
	virtual ~EventHandler(){};

	void createSlot(String name,Delegate dlg)
	{
		mSlots[name] = Slot(dlg);
	}

	Slot& getSlot(String name)
	{
		return mSlots[name];
	}

	bool hasSlot(String name)
	{
		if(mSlots.find(name)!=mSlots.end())
			return true;
		else
			return false;
	}

	void createSignal(String name)
	{
		mSignals[name] = Signal();
	}

	bool hasSignal(String name)
	{
		if(mSignals.find(name)!=mSignals.end())
			return true;
		else
			return false;
	}

	Signal& getSignal(String name)
	{
		return mSignals[name];
	}

protected:

	std::map<String,Slot> mSlots;
	std::map<String,Signal> mSignals;
};

#endif