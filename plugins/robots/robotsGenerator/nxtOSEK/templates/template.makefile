# Target specific macros
TARGET = @@PROJECT_NAME@@_OSEK
TARGET_SOURCES = \
	@@PROJECT_NAME@@.c \
	balancer_param.c \

TOPPERS_OSEK_OIL_SOURCE = ./@@PROJECT_NAME@@.oil

USER_LIB = nxtway_gs_balancer

# Don't modify below part
O_PATH ?= build
include ../nxtOSEK/ecrobot/ecrobot.mak
