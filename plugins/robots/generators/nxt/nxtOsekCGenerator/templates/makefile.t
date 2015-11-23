# Target specific macros
TARGET = @@PROJECT_NAME@@
TARGET_SOURCES = \
	@@PROJECT_NAME@@.c

TOPPERS_OSEK_OIL_SOURCE = ./@@PROJECT_NAME@@.oil

@@BMPFILES@@

# Don't modify below part
O_PATH ?= build
include $(NXT_TOOLS_DIR_POSIX)/nxtOSEK/ecrobot/ecrobot.mak
