#ifndef THREECAM_UI_FREECAMHUD_HPP
#define THREECAM_UI_FREECAMHUD_HPP

#include <ml/cxx/fixedvec.hpp>

/// The max amount of notifications which are displayed on the HUD.
const static u32 kMaxNotifications = 4;

class HUDRenderState; // Internal class

/// The freecam HUD. Displays
class FreecamHUD {
	struct Notification {
		char* textStr; // NOTE: mlStrDup()'d from caller
		u32 tickCounter;
		u32 tickLength;

		void purge();
		float lerpTime();
	};

	HUDRenderState* pRenderState;

	Notification activeNotifications[kMaxNotifications];
	u32 activeNotificationCount;

	/// Allocates a new notification. Returns nil if no notification slots are free.
	Notification* allocNotification();

	/// Removes a previously allocated notification.
	void removeNotification(Notification* pNotif);

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
