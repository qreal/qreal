QT += xml widgets

LIBS += -L../../bin -lqrkernel -lqrutils

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/
LIBS += -L../../bin

HEADERS += \
	test.h


SOURCES += \
    test.cpp \

RESOURCES += \
