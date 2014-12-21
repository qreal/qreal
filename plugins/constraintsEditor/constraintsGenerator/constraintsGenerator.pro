QT += xml widgets

TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../../../bin/plugins/tools/
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrmc

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/constraintsGenerator_ru.ts

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







