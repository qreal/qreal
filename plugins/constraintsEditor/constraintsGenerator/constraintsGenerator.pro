TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

QT += widgets

LIBS += -L../../../bin -lqrkernel -lqrutils

OBJECTS_DIR = .obj
MOC_DIR = .moc

TRANSLATIONS = constraintsGenerator_ru.ts

RESOURCES = \
	constraintsGenerator.qrc \

HEADERS = \
	constraintsGeneratorPlugin.h \
	generator.h \
	concreateGenerator.h \

SOURCES = \
	constraintsGeneratorPlugin.cpp \
	generator.cpp \
	concreateGenerator.cpp \

win32 {
	QMAKE_POST_LINK = "xcopy templates ..\\..\\..\\bin\\templates /s /e /q /y /i "
}
else {
	QMAKE_POST_LINK = "mkdir ../../../bin/templates/ && cp -r templates ../../../bin/templates/ "
}







