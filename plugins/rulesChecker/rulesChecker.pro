QT += xml widgets
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/

LIBS += -L../../bin -lqrkernel -lqrutils

HEADERS += \
	rulesChecker.h \
	rulesPlugin.h \

SOURCES += \
	rulesChecker.cpp \
	rulesPlugin.cpp \

RESOURCES += \
	icons/icons.qrc \

