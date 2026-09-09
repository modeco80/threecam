#include <bx/camera.h>
#include <bx/input.h>
#include <bx/miscapp.h>

#include "utils/hook/classhook.hpp"
#include "utils/log.hpp"

bool freecamEnabled = false;
cCamera* manualCam = nil(cCamera*);

float restoreFov;

// TODO
// EditorZoomIn = L2.held; fov -= (inputs[0]->getState(eInputState_EditorZoomIn) * 10.f)
// EditorZoomOut = L1.held; fov += (inputs[0]->getState(eInputState_EditorZoomIn) * 10.f)

void freecamHandleInputs() {
	if(SSXApp->inputs[0]->getState(eInputState_EditorHelp)) {
		SSXApp->game->ai->riders[0]->state[0] = 3;
		SSXApp->game->ai->riders[0]->substate[0] = 1;
		SSXApp->game->ai->riders[0]->setPosition(manualCam->getPosition());
	}
}

void freecamResetPosition() {
	t4Vector pos = SSXApp->game->ai->riders[0]->position;
	pos.v[2] += 400.f;
	manualCam->pManualCamController->pManualAlgo->position = pos;
}

CLASS_HOOK0(void, cCamera, update) {
	// Handle activation button.
	if(SSXApp->inputs[0]->getState(eInputState_CameraActivate)) {
		freecamEnabled = !freecamEnabled;
		//utilLogf(LogInfo, "3Cam: Free camera is %s.", freecamEnabled ? "ENABLED" : "DISABLED");
		if(freecamEnabled) {
			manualCam = klass;
			restoreFov = klass->fov;
			freecamResetPosition();
			klass->setCurrentController(eCamController_Manual);
			cPauseState::push(PauseState_ManualCam);
		} else {
			// Restore FOV
			klass->fov = restoreFov;
			restoreFov = 0.f;
			// Reset camera back to chase controller and unpause the game
			manualCam = nil(cCamera*);
			klass->setCurrentController(eCamController_Chase);
			cPauseState::pop();
		}
	}

	if(freecamEnabled) {
		freecamHandleInputs();
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
