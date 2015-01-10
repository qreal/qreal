QT += xml widgets

TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../../../bin/plugins/tools/
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrgui-preferences-dialog

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/deploymentInterpreter_ru.ts

RESOURCES = deploymentInterpreter.qrc

HEADERS += \
	$$PWD/deploymentInterpreterPlugin.h \

SOURCES += \
	$$PWD/deploymentInterpreterPlugin.cpp \
