#include <ml/mem.h>
#include <bx/memman.h>

void* mlMalloc(u32 size) {
	return bxNew(size);
}

void mlFree(void* pv) {
	bxDelete(pv);
}
