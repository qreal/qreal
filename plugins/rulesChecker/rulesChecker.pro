include(../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$DESTDIR/plugins/tools

links(qrkernel qrutils)

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



