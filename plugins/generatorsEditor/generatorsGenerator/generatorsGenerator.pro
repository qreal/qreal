TEMPLATE = lib
CONFIG += plugin c++11
DESTDIR = ../../../bin/plugins/

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

QT += widgets

LIBS += -L../../../bin -lqrkernel -lqrutils

OBJECTS_DIR = .obj
MOC_DIR = .moc

HEADERS = \
	generatorsGeneratorPlugin.h \
	generator.h \
	behavioralGenerator.h \
	structuralGenerator.h \
	myAbstractGenerator.h \

SOURCES = \
	generatorsGeneratorPlugin.cpp \
	generator.cpp \
	behavioralGenerator.cpp \
	structuralGenerator.cpp \
	myAbstractGenerator.cpp \

win32 {
	QMAKE_POST_LINK = "xcopy templates ..\\..\\..\\bin\\templates /s /e /q /y /i "
}
else {
	QMAKE_POST_LINK = "mkdir ../../../bin/templates/ && cp -r templates ../../../bin/templates/ "
}
