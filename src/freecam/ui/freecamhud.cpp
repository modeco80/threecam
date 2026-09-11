#include "freecamhud.hpp"
#include <ml/mem.h>
#include <ml/abort.h>
#include <ml/string.h>
#include <bx/math.h>

#include <bx/camera.h>
#include <bx/font.h>
#include <bx/miscapp.h>
#include "../bxhelpers.hpp"

#include "hudrenderstate.hpp"
#include "notificationmanager.hpp"

extern cCamera* manualCam;

// HUD colors
const static tARGB HUD_COLOR = tARGB(1.f, (214.f/255.f), (141.f/255.f), (6.f/255.f));
const static tARGB HUD_SHADOW_COLOR = tARGB(1.f, (84.f/255.f), (57.f/255.f), (2.f/255.f));
const static tARGB NOTIFICATION_DEAD_COLOR = tARGB(0.1f, (84.f/255.f), (57.f/255.f), (2.f/255.f));
const static tARGB NOTIFICATION_DEAD_SHADOW_COLOR = tARGB(0.1f, (33.f/255.f), (23.f/255.f), (2.f/255.f));

inline f32 lerpf(f32 start, f32 end, f32 t) {
	return start + (t * (end-start));
}

/// Lerps two colors.
inline tARGB argbLerp(const tARGB& startColor, const tARGB& endColor, f32 t) {
	return tARGB(lerpf(startColor.a, endColor.a, t), lerpf(startColor.r, endColor.r, t), lerpf(startColor.g, endColor.g, t), lerpf(startColor.b, endColor.b, t));
}

/*static*/ void FreecamHUD::renderNotificationCbThunk(const Notification& notif, u32 index, void* user) {
	static_cast<FreecamHUD*>(user)->renderNotification(notif, index);
}

FreecamHUD::FreecamHUD() {
	pRenderState = new HUDRenderState();
	pNotificationManager = new NotificationManager();
}

FreecamHUD::~FreecamHUD() {
	delete pNotificationManager;
	delete pRenderState;
}

void FreecamHUD::renderNotification(const Notification& notif, u32 index) {
	pRenderState->setFontColor(argbLerp(HUD_COLOR, NOTIFICATION_DEAD_COLOR, notif.lerpTime()));
	pRenderState->setShadowColor(argbLerp(HUD_SHADOW_COLOR, NOTIFICATION_DEAD_SHADOW_COLOR, notif.lerpTime()));
	pRenderState->getFont()->text(10.f, 320.f + (index * 20.f), notif.getString());
}

void FreecamHUD::update() {
	pNotificationManager->update();
}

void FreecamHUD::render() {
	const t4Vector& riderPos = getRider(0)->position;
	const t4Vector& cameraPos = manualCam->pManualCamController->pManualAlgo->position;

	// Begin rendering
	pRenderState->save();
		// Setup font rendering
		pRenderState->setFontColor(HUD_COLOR);
		pRenderState->setShadowColor(HUD_SHADOW_COLOR);
		pRenderState->setShadowOffset(t2Vector(2.f, 1.f));
		pRenderState->setScale(t2Vector(1.f, 1.f));

		pRenderState->getFont()->text(10.f, 20.f, "3Cam Active");
		pRenderState->getFont()->textf(10.f, 40.f, "Rider pos (%0.4f, %0.4f, %0.4f)", riderPos.x(), riderPos.y(), riderPos.z());
		pRenderState->getFont()->textf(10.f, 60.f, "Camera pos (%0.4f, %0.4f, %0.4f)", cameraPos.x(), cameraPos.y(), cameraPos.z());

		// Render notifications.
		pNotificationManager->forEachPfn(&FreecamHUD::renderNotificationCbThunk, static_cast<void*>(this));
	pRenderState->restore();
}

void FreecamHUD::addNotification(const char* pszNotificationText, f32 timeSeconds) {
	pNotificationManager->addNotification(pszNotificationText, timeSeconds);
}

void FreecamHUD::clearNotifications() {
	pNotificationManager->clearNotifications();
}
