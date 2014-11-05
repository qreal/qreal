TEMPLATE = lib
CONFIG += c++11

DESTDIR = ../../../bin/
TARGET = robots-utils

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc
UI_DIR = .ui

include(src/utils.pri)
