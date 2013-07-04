# Target specific macros
TARGET = @@PROJECT_NAME@@_OSEK
TARGET_SOURCES = \
	@@PROJECT_NAME@@.c \
	@@BALANCER@@

TOPPERS_OSEK_OIL_SOURCE = ./@@PROJECT_NAME@@.oil

@@BMPFILES@@

@@BALANCER_LIB@@

# Don't modify below part
O_PATH ?= build
include $(QREALDIRPOSIX)/nxt-tools/nxtOSEK/ecrobot/ecrobot.mak
