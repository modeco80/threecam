#include <bx/camera.h>
#include <bx/input.h>
#include <bx/miscapp.h>

#include "utils/hook/classhook.hpp"
#include "utils/log.hpp"

bool freecamEnabled = false;
cCamera* manualCam = nil(cCamera*);

void freecamInputs() {
	if(SSXApp->inputs[0]->getState(eInputState_EditorHelp)) {
		// TODO set rider position
		utilLogf(LogInfo, "3Cam: TODO");
	}
}

CLASS_HOOK0(void, cCamera, update) {
	// Handle activation button.
	if(SSXApp->inputs[0]->getState(eInputState_CameraActivate)) {
		freecamEnabled = !freecamEnabled;
		utilLogf(LogInfo, "3Cam: Free camera is %s.", freecamEnabled ? "ENABLED" : "DISABLED");
		if(freecamEnabled) {
			manualCam = klass;
			klass->setCurrentController(eCamController_Manual);
			cPauseState::push(PauseState_ManualCam);
		} else {
			// Reset camera back to chase controller and unpause the game
			manualCam = nil(cCamera*);
			klass->setCurrentController(eCamController_Chase);
			cPauseState::pop();
		}
	}

	if(freecamEnabled) {
		freecamInputs();
	}

	hook_cCamera_update.original(klass);
}

extern "C" int modMain() {
	utilLog(LogInfo, "Hello from 3cam");
	if(!hook_cCamera_update.hook()) {
		utilLog(LogErr, "3Cam: failed to hook cCamera::update? Mod will not function");
		return 1;
	}
	return 0;
}
