#ifndef STATE_H
#define STATE_H

#include "stdafx.h"

class State : public EventHandler, public OIS::KeyListener, public OIS::MouseListener
{
public:

	State()
	{
		for(uint i=0;i<238;++i)
			mKeyStates[i] = false;
		for(uint i=0;i<8;++i)
			mMouseStates[i] = false;
	}

	virtual void init() = 0;
	virtual void deinit() = 0;
	virtual bool update(Real delta) = 0;

	// MouseListener
	bool State::mouseMoved(const OIS::MouseEvent &evt) {return true;} 

	bool State::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID btn) 
	{
		mMouseStates[btn] = true;
		return true;
	}

	bool State::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID btn) 
	{
		mMouseStates[btn] = false;
		return true;
	}
	 
	// KeyListener
	bool State::keyPressed(const OIS::KeyEvent &evt) 
	{
		mKeyStates[evt.key] = true;
		return true;
	}

	bool State::keyReleased(const OIS::KeyEvent &evt) 
	{
		mKeyStates[evt.key] = false;
		return true;
	}

	bool isKeyDown(uint key){return mKeyStates[key];};
	bool isMouseDown(uint button){return mMouseStates[button];};

protected:

	bool mKeyStates[238];
	bool mMouseStates[8];

};

#endif