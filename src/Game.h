#ifndef GAME_H
#define GAME_H

#include "stdafx.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "State.h"
#include "InputManager.h"

class Bucket;
class GameObject;

// simple state machine and such, it's a singleton
// also handles timing, logging, scripting and other random utility stuff
// also: audio, since irrklang handles most everything, 
// so a separate audiomanager would be a waste
class Game : public OIS::KeyListener, public OIS::MouseListener
{
public:

    Game();
    ~Game();

    static Game* getPtr();
    static Game& getRef();

    void init();
    void deinit();
    void go();

    void log(String message);

    Real getDeltaTime();
    Real getTimeSpeed();
    void setTimeSpeed(Real speed);
    String getTimeStamp();

	// scripting functions
	void runString(String script);
	void runFile(String filename);

	template<typename T> void registerFunction(T t,String name)
	{
		mChai.add(chaiscript::fun(t),name);
	}

	template<typename T> void registerType(String name)
	{
		mChai.add(chaiscript::user_type<T>(), name);
	}

	void addState(State* state);

	GraphicsManager* getGraphicsManager();
	InputManager* getInputManager(){return mInputMgr;};
	PhysicsManager* getPhysicsManager(){return mPhysicsManager;};

	bool keyPressed(const OIS::KeyEvent &evt);
	bool keyReleased(const OIS::KeyEvent &evt);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID btn);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID btn);

	void pushConsole(String out);

	void registerObject(GameObject* obj,String name,String bucket);
	void createBucket(String name);

	irrklang::ISoundEngine* mAudio;

	bool mShutdown;


protected:

    static Game* mInstance;
    bool mInitialized;
	bool mEndState;

    bool mLog;
    bool mFileLog;
    std::ofstream mLogFile;

    long mLastTime;
    Real mDeltaTime;
    Real mTimeSpeed;

	chaiscript::ChaiScript mChai;


	InputManager* mInputMgr;

	GraphicsManager* mGraphicsManager;

	PhysicsManager* mPhysicsManager;

	std::vector<State*> mStates;

	bool mConsoleActive;
	Gorilla::MarkupText* mConsoleIn;
	std::vector<Gorilla::MarkupText*> mConsoleOut;
	std::vector<String> mConsoleHistory;
	uint mHistoryPos;

	std::map<String,Bucket*> mBuckets;

};

#endif
