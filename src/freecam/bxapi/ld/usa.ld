/* FIXME: When this gets a bit fat, it might be time to split into more ld files. */
/* Also, this is parsed by a very shoddy parser. Don't worry */

/* C library */
strlen = 0x00416810;
memcpy = 0x0041605c;
memset = 0x00416210;
sprintf = 0x00417828;
vsprintf = 0x004186c8;

/* BX */

ModHookPoint = 0x0031afb4;

/* Memory allocation. See bx/memman.h for what these are actually */
bxNew__FUi = 0x00317d20;
bxDelete__FPv = 0x00317e50;

/* TODO cCamera et cetera */

/* TODO cPauseState variables. we will need to make an internal version of push/pop since it got inlined */
