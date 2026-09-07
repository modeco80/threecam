#include <bx/memman.h>
#include <ml/mem.h>

void* mlMalloc(u32 size) {
	return bxNew(size);
}

void mlFree(void* pv) {
	bxDelete(pv);
}
