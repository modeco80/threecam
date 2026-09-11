/* FIXME: When this gets a bit fat, it might be time to split into more ld files. */
/* Also, this is parsed by a very shoddy parser. Don't worry */

/* C library */
strlen = 0x00416810;
memcpy = 0x0041605c;
memset = 0x00416210;
sprintf = 0x00417828;
vsprintf = 0x004186c8;

/* libgcc */
/* We use a similar enough compiler that re-using libgcc functions is "OK" */
fptodp = 0x00413af8;

/* BX */

ModHookPoint = 0x0031afb4;

SSXApp = 0x004a28a8;

gHudFlags = 0x00535c12;

/* Memory allocation. See bx/memman.h for what these are actually */
bxNew__FUi = 0x00317d20;
bxDelete__FPv = 0x00317e50;

/* cCamera */
update__7cCamera = 0x0015df98;
setCurrentController__7cCamera18eCamControllerType = 0x0015dfd8;

/* cPauseState variables */
_11cPauseState$FlagsTable = 0x004428f0;
_11cPauseState$Flags = 0x004a2a50;
_11cPauseState$StackIdx = 0x004a2a54;
_11cPauseState$Stack = 0x005366e8;

/* cInput */
getState__6cInput11eInputState = 0x00320c48;
getValue__6cInput11eInputState = 0x00320bf0;

/* cGame */
load__5cGame = 0x0022ebc8;
purge__5cGame = 0x0022fa98;
update__5cGame = 0x002306b8;
render__5cGame = 0x0022b008;

/* cFont */
text__5cFontffPCc = 0x00391cb0;
textf__5cFontffPCce = 0x001dc990;

/* cRider */
putAt__6cRiderRC8t4VectorT1fUi = 0x0011d660;
cleanupAfterPutAt__6cRiderb = 0x0011df18;
