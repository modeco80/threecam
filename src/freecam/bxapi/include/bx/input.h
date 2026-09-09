#ifndef BX_INPUT_H
#define BX_INPUT_H

#include <ml/types.h>

enum eInputState {
	eInputState_CameraActivate = 0x2f,
	// Will be reused as "put rider at position" input at a later date
	eInputState_EditorHelp = 0x44, // select
	eInputState_EditorZoomIn = 0x4b, // l2
	eInputState_EditorZoomOut = 0x4c, // l1
	eInputState_CVToggleDisplayMode = 0x5b, // triangle
	eInputState_VoipOnOff = 0xc8 // r3

};

class cInput {
   public:
	/// Gets the raw value of a state.
	float getValue(eInputState state);

	/// Gets a boolean set to true if the value computed by the
	/// input map is not .0f.
	bool getState(eInputState state);
};

#endif
