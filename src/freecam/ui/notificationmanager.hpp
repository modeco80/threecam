#ifndef THREECAM_UI_NOTIFICATIONMANAGER_HPP
#define THREECAM_UI_NOTIFICATIONMANAGER_HPP

#include <ml/types.h>
#include "staticstringslotpool.hpp"

/// The max amount of notifications which are displayed on the HUD.
const static u32 kMaxNotifications = 4;
const static u32 kMaxNotificationTextLength = 64;

class NotificationManager;

class Notification {
	friend class NotificationManager;
	NotificationManager* pManager;
	char* textStr;
	u32 tickCounter;
	u32 tickLength;

	void purge();

public:
	const char* getString() const { return textStr; }

	/// Returns a value from 0 to 1.0 which repressents
	/// when to fade out the notification.
	float lerpTime() const;
};

class NotificationManager {
	friend class Notification;
	StaticStringSlotPool<kMaxNotifications, kMaxNotificationTextLength-1> notifStringPool;

	Notification activeNotifications[kMaxNotifications];
	u32 activeNotificationCount;

	void freeString(char* psz);

	/// Allocates a new notification.
	Notification* allocNotification();

	/// Removes a previously allocated notification.
	void removeNotification(Notification* pNotif);
public:
	NotificationManager();
	~NotificationManager();

	void update();
	void forEachPfn(void(*pcb)(const Notification& notif, u32 index, void* user), void* user);

	void addNotification(const char* pszNotificationText, f32 timeSeconds = 2.5f);

	/// Clears all notifications.
	void clearNotifications();

};

#endif
