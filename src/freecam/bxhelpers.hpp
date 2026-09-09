#ifndef THREECAM_BXHELPERS_HPP
#define THREECAM_BXHELPERS_HPP

#include <bx/miscapp.h>

// Helpers to get at various bits n bobs easier.

inline cGame* getGame() {
	return SSXApp->game;
}

inline cRider* getRider(u32 index) {
	return getGame()->ai->riders[0];
}

inline cInput* getInput(u32 index) {
	return SSXApp->inputs[0];
}

#endif
