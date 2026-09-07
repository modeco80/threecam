NAME = ssx_3_cam_$(REGION)
TYPE := elf
DEBUG ?= 0

# Runtime sources (startup code, and code
# needed to link the ml libs)
RUNTIME := \
	crt0.S \
	runtime/ml_memory.cpp \
	runtime/ml_abort.cpp

UTILS := \
	utils/log.cpp \
	utils/hook/trampoline.cpp

SOURCES := \
	$(RUNTIME) \
	$(UTILS) \
	main.cpp

LINKSCRIPT = $(OBJDIR)/freecam_blob.ld
LINKLIBS := \
	../ml/core/obj/libml3_core.a \
	../ml/cxx/obj/libml3_cxx.a \
	../ml/hw/obj/libml3_hw.a

# User C/CXX flags.
USER_CFLAGS = -DMOD_REGION_$(REGION) -I bxapi/include
USER_CXXFLAGS := \
	-Wno-pmf-conversions

# Release builds (to remove mlASSERTs:)
ifeq ($(DEBUG),0)
USER_CFLAGS += -DNDEBUG
endif

# Pre deps
EXTRATARGETS_PRE = $(LINKSCRIPT)
EXTRATARGETS_POST = $(GAME_$(GAME)_$(REGION)_PNACH)

# All files that make up the mkpnach tool.
# Used to make sure that we rebuild the pnach if the mkpnach Python code is changed.
MKPNACH_DEPS := \
	$(TOP)/tools/mkpnach.py \
	$(TOP)/tools/utils/elf.py \
	$(TOP)/tools/utils/ldparse.py \
	$(TOP)/tools/utils/mips.py \
	$(TOP)/tools/utils/pnach.py

include $(TOP)/mk/makedef.mk

# Preprocess the linker script to include game symbols here.
$(OBJDIR)/freecam_blob.ld: $(OBJDIR)/ freecam_blob.ld.in
	$(info Preprocessing linker script $@)
	sed 's|@REGION@|$(REGION)|' freecam_blob.ld.in > $@

$(GAME_$(GAME)_$(REGION)_PNACH): bin/$(NAME).elf $(MKPNACH_DEPS)
	$(info Making PNACH $@)
	$(TOP)/tools/mkpnach.py $@ $(REGION)

