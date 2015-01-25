TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../bin/plugins/tools/
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqslog -lqrgui-preferences-dialog

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

QT += widgets

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/uxInfo_ru.ts

HEADERS = \
	$$PWD/uxInfoPlugin.h \
	$$PWD/uxInfo.h \
	$$PWD/filterObject.h \

SOURCES = \
	$$PWD/uxInfoPlugin.cpp \
	$$PWD/uxInfo.cpp \
	$$PWD/filterObject.cpp \

RESOURCES += \
	$$PWD/uxInfo.qrc \
