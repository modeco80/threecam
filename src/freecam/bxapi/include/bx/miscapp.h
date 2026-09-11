#ifndef BX_MISCAPP_H
#define BX_MISCAPP_H

#include <bx/math.h>

// dumping ground for misc. things

class cInput;
class cFont;

class cRider {
public:
	u32 state;
	u32 substate;
	u8 HACKpad[0x58];
	u8 pad[0xb0];
	t4Vector position;

	void setPosition(const t4Vector& pos) {
		position = pos;
	}

	void putAt(const t4Vector& v1, const t4Vector& v2, float funk, u32 animId);
};

class cAI {
public:
	u8 pad[0x28];
	cRider* riders[6];
};

class cGame {
public:
	u8 pad[0xc];
	cAI* ai;
	u8 pad2[0x14];
	cFont* debugFont;

	void load();
	void purge();
	void update();
	i32 render();
};

ML_STATIC_ASSERT(ml_offsetof(cGame, debugFont) == 0x24);

class cSSXApp {
   public:
	u8 pad[0x84];
	cGame* game;
	u8 pad2[0x28];
	cInput* inputs[2];
};

// Make sure cSSXApp is valid
ML_STATIC_ASSERT(ml_offsetof(cSSXApp, game) == 0x84);
ML_STATIC_ASSERT(ml_offsetof(cSSXApp, inputs[0]) == 176);

extern cSSXApp* SSXApp;

// cPauseState

enum tPauseState {
	PauseState_Paused = 3,
	PauseState_ManualCam = 7
};

const static u32 kPauseStackSize = 7;

class cPauseState {
	static const i32 FlagsTable[8];
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
		// should be this, but it seems to make things unhappy and "hang". i might be stupid
		// Flags = FlagsTable[Stack[StackIdx]];
		Flags = 0;
	}
};

#endif
