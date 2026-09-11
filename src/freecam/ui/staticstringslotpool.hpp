#ifndef THREECAM_UI_STATICSTRINGSLOTPOOL_HPP
#define THREECAM_UI_STATICSTRINGSLOTPOOL_HPP

#include <ml/string.h>
#include <ml/cxx/bitset.hpp>

/// A basic string pool with static slots.
template <u32 nMaxSlots, u32 nMaxLength>
class StaticStringSlotPool {
	struct Slot {
		char szString[nMaxLength+1];
	};

	Slot slots[nMaxSlots];
	ml::BitSet<nMaxSlots> slotUsageSet;

	inline Slot* allocSlot() {
		for(u32 i = 0; i < nMaxSlots; ++i) {
			if(!slotUsageSet[i]) {
				slotUsageSet.set(i);
				return &slots[i];
			}
		}
	}

	inline void freeSlot(Slot* pSlot) {
		const u32 index = (pSlot - &slots[0]);
		if(slotUsageSet[index]) {
			slotUsageSet.unset(index);
		}
	}

public:

	inline char* allocCopyString(const char* psz) {
		// Try
		Slot* slot = allocSlot();
		if(slot == nil(Slot*))
			return nil(char*);

		// Trim length
		u32 nCopyLength = strlen(psz);
		if(nCopyLength >= nMaxLength) {
			nCopyLength = nMaxLength;
		}

		memcpy(&slot->szString[0], &psz[0], nCopyLength);
		slot->szString[nCopyLength] = '\0';
		return &slot->szString[0];
	}

	inline void free(char* psz) {
		// Pointer is the same as the start of a slot struct
		// so we can safely use it
		freeSlot(reinterpret_cast<Slot*>(psz));
	}
};

#endif
