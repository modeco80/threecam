#include <bx/camera.h>
#include <bx/input.h>
#include <bx/miscapp.h>

#include "ui/freecamhud.hpp"
#include "utils/hook/classhook.hpp"
#include "utils/log.hpp"

#include "bxhelpers.hpp"

// TODO this probably should be a class at this point.
bool freecamEnabled = false;
bool hudEnabled = true;
cCamera* manualCam = nil(cCamera*);
cInput* freecamInput = nil(cInput*);
FreecamHUD* freecamHUD = nil(FreecamHUD*);
u16 lastHudFlags = 0;
extern u16 gHudFlags;


void freecamResetPosition() {
	t4Vector pos = getRider(0)->position;
	// Push Z up a bit.
	pos.v[2] += 400.f;
	manualCam->pManualCamController->pManualAlgo->position = pos;
}

void freecamHandleInputs() {
	if(freecamInput->getState(eInputState_EditorHelp)) {
		cRider* pRider = getRider(0);
		pRider->state = 0;
		pRider->substate = 0;
		//pRider->putAt(manualCam->getPosition(), manualCam->getPosition(), 0., 0);
		pRider->setPosition(manualCam->getPosition());

		if(hudEnabled) {
			freecamHUD->addNotification("Snapped rider to camera position");
		}
	}

	if(freecamInput->getState(eInputState_TestTerrainType)) {
		hudEnabled = !hudEnabled;
		if(!hudEnabled) {
			freecamHUD->clearNotifications();
		}
	}

	if(freecamInput->getState(eInputState_VoipOnOff)) {
		freecamResetPosition();

		if(hudEnabled) {
			freecamHUD->addNotification("Snapped camera to rider position");
		}
	}
}

void freecamEnable(cCamera* pCamera) {
	freecamInput = getInput(0);
	manualCam = pCamera;
	lastHudFlags = gHudFlags;
	gHudFlags = 0xe;
	freecamResetPosition();
	pCamera->setCurrentController(eCamController_Manual);
	cPauseState::push(PauseState_ManualCam);
}

void freecamDisable() {
	// Reset camera back to chase controller and unpause the game
	manualCam->setCurrentController(eCamController_Chase);
	manualCam = nil(cCamera*);
	freecamInput = nil(cInput*);
	gHudFlags = lastHudFlags;
	lastHudFlags = 0;
	cPauseState::pop();
}

CLASS_HOOK0(void, cGame, load) {
	hook_cGame_load.original(klass);
	// Allocate the freecam HUD.
	freecamHUD = new FreecamHUD();
}

CLASS_HOOK0(void, cGame, purge) {
	// Free it.
	delete freecamHUD;
	freecamHUD = nil(FreecamHUD*);
	hook_cGame_purge.original(klass);
}

CLASS_HOOK0(void, cGame, update) {
	hook_cGame_update.original(klass);
	if(freecamEnabled && hudEnabled) {
		freecamHUD->update();
	}
}

CLASS_HOOK0(i32, cGame, render) {
	i32 orig = hook_cGame_render.original(klass);
	// Render HUD if enabled
	if(freecamEnabled && hudEnabled) {
		freecamHUD->render();
	}
	return orig;
}

CLASS_HOOK0(void, cCamera, update) {
	// Handle activation button.
	if(getInput(0)->getState(eInputState_CameraActivate)) {
		freecamEnabled = !freecamEnabled;
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
	utilLog(LogInfo, "Hello from 3cam!");
	// Hook game functions

	if(!hook_cCamera_update.hook()) {
		utilLog(LogErr, "Failed to hook cCamera::update().");
		return 1;
	}
	if(!hook_cGame_load.hook()) {
		utilLog(LogErr, "Failed to hook cGame::load().");
		return 1;
	}
	if(!hook_cGame_purge.hook()) {
		utilLog(LogErr, "Failed to hook cGame::purge().");
		return 1;
	}
	if(!hook_cGame_update.hook()) {
		utilLog(LogErr, "Failed to hook cGame::update().");
		return 1;
	}
	if(!hook_cGame_render.hook()) {
		utilLog(LogErr, "Failed to hook cGame::render().");
		return 1;
	}
	return 0;
}
