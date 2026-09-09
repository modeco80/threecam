#include "freecamhud.hpp"
#include <ml/mem.h>
#include <ml/abort.h>
#include <ml/string.h>
#include <bx/math.h>

#include <bx/camera.h>
#include <bx/font.h>
#include <bx/miscapp.h>
#include "../bxhelpers.hpp"

extern cCamera* manualCam;

#ifdef MOD_REGION_usa
const static u32 kTicksPerSecond = 60;
#endif

inline u32 secondsToTicks(f32 seconds) {
	return static_cast<u32>(seconds * (kTicksPerSecond));
}

inline f32 lerpf(f32 start, f32 end, f32 t) {
	return start + (t * (end-start));
}

/// Lerps two colors.
static tARGB argbLerp(const tARGB& startColor, const tARGB& endColor, f32 t) {
	return tARGB(lerpf(startColor.a, endColor.a, t), lerpf(startColor.r, endColor.r, t), lerpf(startColor.g, endColor.g, t), lerpf(startColor.b, endColor.b, t));
}

const static tARGB HUD_COLOR = tARGB(1.f, (214.f/255.f), (141.f/255.f), (6.f/255.f));
const static tARGB HUD_SHADOW_COLOR = tARGB(1.f, (84.f/255.f), (57.f/255.f), (2.f/255.f));
const static tARGB NOTIFICATION_DEAD_COLOR = tARGB(0.1f, (84.f/255.f), (57.f/255.f), (2.f/255.f));
const static tARGB NOTIFICATION_DEAD_SHADOW_COLOR = tARGB(0.1f, (33.f/255.f), (23.f/255.f), (2.f/255.f));

class HUDRenderState {
	t2Vector savedShadowOffset;
	tARGB savedFontColor;
	tARGB savedShadowColor;
	t2Vector savedScale;

	cFont* pFont;
public:

	HUDRenderState() {
		pFont = getGame()->debugFont;
	}

	void push() {
		savedScale = pFont->scale;
		savedFontColor = pFont->color;
		savedShadowOffset = pFont->shadowOffset;
		savedShadowColor = pFont->shadowColor;
	}

	void setFontColor(const tARGB& color) {
		pFont->color = color;
	}

	void setShadowColor(const tARGB& color) {
		pFont->shadowColor = color;
	}

	void setShadowOffset(const t2Vector& offset) {
		pFont->shadowOffset = offset;
	}

	void setScale(const t2Vector& scale) {
		pFont->scale = scale;
	}

	cFont* getFont() const {  return pFont; }

	void pop() {
		pFont->scale = savedScale;
		pFont->color = savedFontColor;
		pFont->shadowColor = savedShadowColor;
		pFont->shadowOffset = savedShadowOffset;
	}
};

void FreecamHUD::Notification::purge() {
	if(this->textStr != nil(char*))
		mlFree(static_cast<void*>(this->textStr));
	this->textStr = nil(char*);
	this->tickCounter = 0xffffffff;
	this->tickLength = 0xffffffff;
}

float FreecamHUD::Notification::lerpTime() {
	return static_cast<float>(tickCounter) / static_cast<float>(tickLength);
}


FreecamHUD::Notification* FreecamHUD::allocNotification() {
	// If we can't allocate any more notifications...
	if(activeNotificationCount == kMaxNotifications) {
		// let's find the notification that expires the earliest.
		Notification* pOverwriteNotification = &activeNotifications[0];
		for(u32 i = 0; i < activeNotificationCount; ++i) {
			if(activeNotifications[i].tickCounter < pOverwriteNotification->tickCounter)
				pOverwriteNotification = &activeNotifications[i];
		}

		// Purge the found notification and return it.
		pOverwriteNotification->purge();
		return pOverwriteNotification;
	}

	return &activeNotifications[activeNotificationCount++];
}

void FreecamHUD::removeNotification(Notification* pNotif) {
	mlASSERT(pNotif != nil(Notification*));
	// Confusing pointer math basically gives us the position of this object in the activeNotifications
	// array. This is probably stupid but it works.
	const u32 position = (pNotif - &activeNotifications[0]);

	// Purge the notification
	pNotif->purge();

	// Shift other notifications in place
	for(u32 i = position + 1; i < activeNotificationCount; ++i)
		activeNotifications[i - 1] = activeNotifications[i];
	activeNotificationCount--;
}


FreecamHUD::FreecamHUD() {
	activeNotificationCount = 0;
	pRenderState = new HUDRenderState();
}

FreecamHUD::~FreecamHUD() {
	delete pRenderState;
	clearNotifications();
}

void FreecamHUD::update() {
	if(!activeNotificationCount)
		return;

	Notification* removeList[kMaxNotifications];
	u32 removeListLength;

	for(u32 i = 0; i < activeNotificationCount; ++i) {
		if(activeNotifications[i].tickCounter++ == activeNotifications[i].tickLength) {
			removeList[removeListLength++] = &activeNotifications[i];
		}
	}

	for(u32 i = 0; i < removeListLength; ++i) {
		removeNotification(removeList[i]);
	}
}

void FreecamHUD::render() {
	HUDRenderState rs;
	const t4Vector& riderPos = getRider(0)->position;
	const t4Vector& cameraPos = manualCam->pManualCamController->pManualAlgo->position;

	// Begin rendering
	rs.push();
		// Setup font rendering
		rs.setFontColor(HUD_COLOR);
		rs.setShadowColor(HUD_SHADOW_COLOR);
		rs.setShadowOffset(t2Vector(2.f, 1.f));
		rs.setScale(t2Vector(1.f, 1.f));

		rs.getFont()->text(10.f, 20.f, "3Cam Active");
		rs.getFont()->textf(10.f, 40.f, "Rider pos (%0.4f, %0.4f, %0.4f)", riderPos.x(), riderPos.y(), riderPos.z());
		rs.getFont()->textf(10.f, 60.f, "Camera pos (%0.4f, %0.4f, %0.4f)", cameraPos.x(), cameraPos.y(), cameraPos.z());

		// Render notifications.
		if(activeNotificationCount) {
			for(u32 i = 0; i < activeNotificationCount; ++i) {
				rs.setFontColor(argbLerp(HUD_COLOR, NOTIFICATION_DEAD_COLOR, activeNotifications[i].lerpTime()));
				rs.setShadowColor(argbLerp(HUD_SHADOW_COLOR, NOTIFICATION_DEAD_SHADOW_COLOR, activeNotifications[i].lerpTime()));
				rs.getFont()->text(10.f, 320.f + (i * 20.f), activeNotifications[i].textStr);
			}
		}
	rs.pop();
}

void FreecamHUD::addNotification(const char* pszNotificationText, f32 timeSeconds) {
	Notification* pNotification = allocNotification();

	// Currently allocNotification() can't fail due to how it's implemented currently,
	// but I'm keeping this here in case the implementation changes and it can..
#if 0
	// Could not allocate a notification, give up
	if(pNotification == nil(Notification*))
		return;
#endif

	// Set up the notification
	pNotification->textStr = mlStrDup(pszNotificationText);
	pNotification->tickCounter = 0;
	pNotification->tickLength = secondsToTicks(timeSeconds);
}


void FreecamHUD::clearNotifications() {
	// Purge all active notifications and set the count of active notifications to 0.
	for(u32 i = 0 ; i < activeNotificationCount; ++i) {
		activeNotifications[i].purge();
	}
	activeNotificationCount = 0;
}
