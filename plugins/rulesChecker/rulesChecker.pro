QT += widgets

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/

LIBS += -L../../bin -lqrkernel -lqrutils

TRANSLATIONS = rulesChecker_ru.ts

RESOURCES += rulesChecker.qrc \

HEADERS += \
	rulesChecker.h \
	rulesBPMNPlugin.h \
	bpmnCustomizer.h

SOURCES += \
	rulesChecker.cpp \
	rulesBPMNPlugin.cpp \
	bpmnCustomizer.cpp



