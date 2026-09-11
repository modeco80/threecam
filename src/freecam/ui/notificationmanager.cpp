#include "notificationmanager.hpp"
#include <ml/abort.h>
#include <ml/mem.h>
#include <ml/string.h>

// Should probably be in bxhelpers.hpp
// but for now this is the only use of this code so /shrug?
#ifdef MOD_REGION_usa
const static u32 kTicksPerSecond = 60;
#endif

inline u32 secondsToTicks(f32 seconds) {
	return static_cast<u32>(seconds * (kTicksPerSecond));
}


void Notification::purge() {
	if(this->textStr != nil(char*)) {
		pManager->freeString(this->textStr);
		this->textStr = nil(char*);
	}
	this->tickCounter = 0xffffffff;
	this->tickLength = 0xffffffff;
}

float Notification::lerpTime() const {
	return static_cast<float>(tickCounter) / static_cast<float>(tickLength);
}

void NotificationManager::freeString(char* psz) {
	notifStringPool.free(psz);
}

Notification* NotificationManager::allocNotification() {
	// If we can't allocate any more notifications...
	if(activeNotificationCount == kMaxNotifications) {
		// let's find the notification that expires the earliest, and use that.
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

void NotificationManager::removeNotification(Notification* pNotif) {
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

NotificationManager::NotificationManager() {
	activeNotificationCount = 0;
}

NotificationManager::~NotificationManager() {
	clearNotifications();
}

void NotificationManager::update() {
	if(!activeNotificationCount)
		return;

	Notification* removeList[kMaxNotifications];
	u32 removeListLength;

	// Go through every notification and update their tick counter.
	// If it happens that a notification has passed its life time then
	// append the pointer to the notification in the remove list.
	for(u32 i = 0; i < activeNotificationCount; ++i) {
		if(activeNotifications[i].tickCounter++ == activeNotifications[i].tickLength) {
			removeList[removeListLength++] = &activeNotifications[i];
		}
	}

	// Remove all notifications queued for removal.
	for(u32 i = 0; i < removeListLength; ++i) {
		removeNotification(removeList[i]);
	}
}

void NotificationManager::forEachPfn(void(*pcb)(const Notification& notif, u32 index, void* user), void* user) {
	for(u32 i = 0 ; i< activeNotificationCount; ++i) {
		pcb(activeNotifications[i], i, user);
	}
}

void NotificationManager::addNotification(const char* pszNotificationText, f32 timeSeconds) {
	Notification* pNotification = allocNotification();

	// Currently allocNotification() can't fail due to how it's implemented currently,
	// but I'm keeping this here in case the implementation changes and it can..
#if 0
	// Could not allocate a notification, give up
	if(pNotification == nil(Notification*))
		return;
#endif

	// Set up the notification
	pNotification->pManager = this;
	pNotification->textStr = notifStringPool.allocCopyString(pszNotificationText);
	pNotification->tickCounter = 0;
	pNotification->tickLength = secondsToTicks(timeSeconds);
}


void NotificationManager::clearNotifications() {
	// Purge all active notifications and set the count of active notifications to 0.
	for(u32 i = 0 ; i < activeNotificationCount; ++i) {
		activeNotifications[i].purge();
	}
	activeNotificationCount = 0;
}
