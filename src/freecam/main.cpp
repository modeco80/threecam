#include <bx/camera.h>
#include <bx/input.h>
#include <bx/miscapp.h>

#include "utils/hook/classhook.hpp"
#include "utils/log.hpp"

#include "bxhelpers.hpp"

bool freecamEnabled = false;
cCamera* manualCam = nil(cCamera*);
cInput* freecamInput = nil(cInput*);

void freecamResetPosition() {
	t4Vector pos = getRider(0)->position;
	// Push Z up a bit.
	pos.v[2] += 400.f;
	manualCam->pManualCamController->pManualAlgo->position = pos;
}

void freecamHandleInputs() {
	if(freecamInput->getState(eInputState_EditorHelp)) {
		cRider* pRider = getRider(0);
		pRider->state[0] = 3;
		pRider->substate[0] = 1;
		pRider->setPosition(manualCam->getPosition());
	}

	if(freecamInput->getState(eInputState_VoipOnOff)) {
		freecamResetPosition();
	}
}

void freecamEnable(cCamera* pCamera) {
	freecamInput = getInput(0);
	manualCam = pCamera;
	freecamResetPosition();
	pCamera->setCurrentController(eCamController_Manual);
	cPauseState::push(PauseState_ManualCam);
}

void freecamDisable() {
	// Reset camera back to chase controller and unpause the game
	manualCam->setCurrentController(eCamController_Chase);
	manualCam = nil(cCamera*);
	freecamInput = nil(cInput*);
	cPauseState::pop();
}

CLASS_HOOK0(void, cCamera, update) {
	// Handle activation button.
	if(getInput(0)->getState(eInputState_CameraActivate)) {
		freecamEnabled = !freecamEnabled;
		//utilLogf(LogInfo, "3Cam: Free camera is %s.", freecamEnabled ? "ENABLED" : "DISABLED");
		if(freecamEnabled) {
			freecamEnable(klass);
		} else {
			freecamDisable();
		}
	}

	// If freecam is enabled, handle its inputs.
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
