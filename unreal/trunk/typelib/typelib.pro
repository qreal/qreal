TEMPLATE = lib
OBJECTS_DIR = .obj
CONFIG += staticlib
TARGET = typelib

HEADERS += \
    type.h \
    value.h \
    typefactory.h \
    constraint.h \
    typelib.h

SOURCES += \
    type.cpp \
    value.cpp \
    typefactory.cpp \
    constraint.cpp
