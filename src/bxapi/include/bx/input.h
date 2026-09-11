#ifndef BX_INPUT_H
#define BX_INPUT_H

#include <ml/types.h>

enum eInputState {
	eInputState_CameraActivate = 0x2f,
	eInputState_EditorHelp = 0x44, // select
	eInputState_EditorZoomIn = 0x4b, // r1
	eInputState_CVToggleDisplayMode = 0x5b, // triangle
	eInputState_TestTerrainType = 0xc3, // l1
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
