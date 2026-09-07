#include <regdef.h>

#define at $1

#define SSX3_HOOK

/*
 * Minimal startup code for mod code.
 *
 * Where we are jumped into, we have a lot of stuff which has been pre-initialized for us
 * so we do not really need to do much of anything.
 */

.set noat
.set noreorder

.text

.global _start
_start:
	/* TODO: zerofill our .bss section. EE Kernel seems to
	 zerofill the area we are linked into for us already,
	 but it might be wise to just do it anyways...
	*/

	/* Save registers which are clobbered on the stack */
#ifdef SSX3_HOOK
	addiu sp, sp, -0x10
#else
	addiu sp, sp, -0x10
#endif
	sw ra, (sp)
	sw gp, 4(sp)
#ifdef SSX3_HOOK
	sw a0, 8(sp)
	sw a1, 0xc(sp)
#endif

	/* Call any C++ global constructors which happen to be registered */
	jal __do_global_ctors
	nop

	/* Now that .bss should hopefully be filled, and
	 we've called global constructors, call the C++ entry point. */
	jal modMain
	nop

	slti at, v0, 0x0
	beq at, zero, .Lokay
	nop

	/* If we get here, chaosMain() > 0, so something went wrong.
		Call global destructors. */
	jal __do_global_dtors
	nop

.Lokay:
	/* Restore registers which were clobbered from the stack */
	lw gp, 4(sp)
	lw ra, (sp)
#ifdef SSX3_HOOK
	lw a1, 0xc(sp)
	lw a0, 0x8(sp)
	/* We overwrite these two instructions in the static trampoline.
	 Therefore, to ensure the game starts up correctly, we need to execute
	 them when returning back. */
	move a2, zero
	move a3, zero
#endif
	/* Return back to the game code. */
	jr ra
	addiu sp, sp, 0x10
