#ifndef LD19_H
#define LD19_H

// standard includes and stuff
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <time.h>
#include <stdio.h>
#include <assert.h>

// save some typing
typedef std::string String;
typedef unsigned int uint;
typedef unsigned long long uint64;
typedef float Real;// so it can switch easily to double precision if need be...

// Ogre
#include "Ogre.h"
#include "Gorilla.h"

// bullet
//#include "PhysicsMasks.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

// OIS
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISJoyStick.h"
#include "OISInputManager.h"

// irrklang
#include "irrKlang.h"

// Chaiscript
#include "chaiscript/chaiscript.hpp"

// fast delegate (for signal/slots)
#include "FastDelegate.h"

// this shouldn't be changing
#include "SignalSlot.h"

#endif
