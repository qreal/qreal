QT += xml widgets
TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/

LIBS += -L../../bin -lqrkernel -lqrutils

HEADERS += \
	rulesChecker.h \
    rulesBPMNPlugin.h

SOURCES += \
	rulesChecker.cpp \
    rulesBPMNPlugin.cpp

RESOURCES += \

