# Target specific macros
TARGET = @@PROJECT_NAME@@_OSEK
TARGET_SOURCES = \
	@@PROJECT_NAME@@.c
TOPPERS_OSEK_OIL_SOURCE = ./@@PROJECT_NAME@@.oil

# Don't modify below part
O_PATH ?= build
include ../nxtOSEK/ecrobot/ecrobot.mak
