QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

TRANSLATIONS = robotsGenerator_ru.ts

RESOURCES = robotsGenerator.qrc

HEADERS += \
	robotsGeneratorPlugin.h \
	nxtOSEK/nxtOSEKRobotGenerator.h \
	nxtOSEK/smartLine.h \
	nxtOSEK/nxtFlashTool.h \

SOURCES += \
	robotsGeneratorPlugin.cpp \
	nxtOSEK/nxtOSEKRobotGenerator.cpp \
	nxtOSEK/smartLine.cpp \
	nxtOSEK/nxtFlashTool.cpp \
