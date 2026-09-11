#ifndef THREECAM_UI_FREECAMHUD_HPP
#define THREECAM_UI_FREECAMHUD_HPP

#include <ml/types.h>

class HUDRenderState; // Internal class
class Notification;
class NotificationManager;

/// The freecam HUD.
class FreecamHUD {
	HUDRenderState* pRenderState;
	NotificationManager* pNotificationManager;

	void renderNotification(const Notification& notif, u32 index);

	static void renderNotificationCbThunk(const Notification& notif, u32 index, void* user);
public:
	FreecamHUD();
	~FreecamHUD();
	void update();
	void render();

	/// Adds a new notification to the heads-up display.
	void addNotification(const char* pszNotificationText, f32 timeSeconds = 2.5f);

	/// Clears all notifications.
	void clearNotifications();
};

#endif
