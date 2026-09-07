GAME := ssx3
REGION ?= usa

# US version
GAME_ssx3_usa_VALID = yes
GAME_ssx3_usa_GCC_VERSION = 2.96-ee-001003-1
GAME_ssx3_usa_PNACH = 08FFF00D.threecam.pnach

# sanity check
ifneq ($(GAME_$(GAME)_$(REGION)_VALID),yes)
$(error The game/version combination $(GAME)/$(REGION) is currently not supported.)
endif
