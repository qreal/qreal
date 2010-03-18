TEMPLATE = lib
OBJECTS_DIR = .obj

HEADERS += \
    type.h \
    value.h \
    typefactory.h

SOURCES += \
    type.cpp \
    value.cpp \
    typefactory.cpp

SUBDIRS=tests
