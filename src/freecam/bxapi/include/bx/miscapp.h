#ifndef BX_MISCAPP_H
#define BX_MISCAPP_H

#include <ml/types.h>

// dumping ground for misc. things

class cInput;

class cSSXApp {
   public:
	u8 pad[0xb0];
	cInput* inputs[2];
};

extern cSSXApp* SSXApp;

// cPauseState

enum tPauseState {
	PauseState_Paused = 3,
	PauseState_ManualCam = 7
};

const static u32 kPauseStackSize = 7;

class cPauseState {
	static i32 FlagsTable[8];
	static i32 StackIdx;
	static i32 Stack[kPauseStackSize];
	static i32 Flags;

   public:
	static void push(tPauseState pause) {
		StackIdx++;
		Stack[StackIdx] = pause;
		Flags = FlagsTable[Stack[StackIdx]];
	}

	static void pop() {
		StackIdx--;
		// should be this, but it seems to hang. i might be stupid
		// Flags = FlagsTable[Stack[StackIdx]];
		Flags = 0;
	}
};

#endif
