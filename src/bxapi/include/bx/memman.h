#ifndef BX_MEMMAN_H
#define BX_MEMMAN_H

#include <ml/types.h>

/// actually ssx's default `operator new`, but I can't be bothered
void* bxNew(u32 size);

void bxDelete(void* p);

#endif
