QT += widgets network

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

TRANSLATIONS = trikGenerator_ru.ts

RESOURCES = trikGenerator.qrc

HEADERS += \
	trikGeneratorPlugin.h \

SOURCES += \
	trikGeneratorPlugin.cpp \

include(robotCommunication/robotCommunication.pri)
