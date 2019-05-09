#pragma once

#include "../global.h"

/**
 * \file read_keys.h
 * \brief Contains functions relating to the user's input being read in. This can be
 *		  used to set up a script to launch when the user presses certain keys.
 */
 
//Functions Hooked In
void ReadKeys(void);

//Exported Data Structures
// keypad struct
struct KeypadSetter 
{
	u16 keyMapToForce;	// key(s) to force
	u16 keysToIgnore;		// key(s) to ignore
	u8 keyForcingCounter;	// number of times to force said key(s)
	u8 keyFlags;			// flags for specific key functions
	u16 keyToRunScript;	// key to enable script to run
	const u8* scriptToRun;	// script to run from specified key
};

#define gKeypadSetter ((struct KeypadSetter*) 0x203B7B0)