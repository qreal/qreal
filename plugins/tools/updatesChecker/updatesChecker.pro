TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../bin/plugins/tools/
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqslog

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

QT += widgets xml

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/updatesChecker_ru.ts

HEADERS = \
	$$PWD/updatesCheckerPlugin.h \
	$$PWD/updater.h \
	$$PWD/updateVersionDialog.h \

SOURCES = \
	$$PWD/updatesCheckerPlugin.cpp \
	$$PWD/updater.cpp \
	$$PWD/updateVersionDialog.cpp \

RESOURCES += \
	$$PWD/updatesChecker.qrc \
