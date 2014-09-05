QT += widgets

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
DESTDIR = ../../bin/plugins/

LIBS += -L../../bin -lqrkernel -lqrutils

INCLUDEPATH += \
	../../ \
	../../qrgui \

TRANSLATIONS = $$PWD/../../qrtranslations/ru/plugins/bmpnRulesChecker_ru.ts

HEADERS += \
	rulesChecker.h \
	rulesBPMNPlugin.h \
	bpmnCustomizer.h

SOURCES += \
	rulesChecker.cpp \
	rulesBPMNPlugin.cpp \
	bpmnCustomizer.cpp



