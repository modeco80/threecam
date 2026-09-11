#ifndef THREECAM_UI_STATICSTRINGSLOTPOOL_HPP
#define THREECAM_UI_STATICSTRINGSLOTPOOL_HPP

#include <ml/string.h>
#include <ml/cxx/bitset.hpp>

/// A basic string pool which allocates with statically allocated slots for each string.
/// [nMaxSlots] template parameter controls how many slots will be allocated.
/// [nMaxLength] template parameter controls the max length of a string for each string slot.
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
	inline char* alloc() {
		// Try allocating a slot. If all slots are free, give up.
		Slot* slot = allocSlot();
		if(slot == nil(Slot*))
			return nil(char*);
		return &slot->szString[0];
	}

	inline char* allocCopyString(const char* psz) {
		// Try allocating the string slot.
		char* pszSlotString = alloc();
		if(pszSlotString == nil(char*))
			return pszSlotString;

		// Gather length of input string. If it's too large,
		// clamp it down to the max string length, effectively
		// causing the string to be truncated.
		u32 nCopyLength = strlen(psz);
		if(nCopyLength >= nMaxLength-1) {
			nCopyLength = nMaxLength;
		}

		// Copy the string into the string slot, null terminating it.
		memcpy(&pszSlotString[0], &psz[0], nCopyLength);
		pszSlotString[nCopyLength] = '\0';

		return pszSlotString;
	}

	inline void free(char* psz) {
		// Pointer is the same as the start of a slot struct
		// so we can safely use it.
		freeSlot(reinterpret_cast<Slot*>(psz));
	}
};

#endif
