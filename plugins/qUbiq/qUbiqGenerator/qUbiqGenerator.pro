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

#TRANSLATIONS = qUbiqGenerator_ru.ts

#RESOURCES = \
#	qUbiqGenerator.qrc \

HEADERS = \
	qUbiqGeneratorPlugin.h \
	generator.h \

SOURCES = \
	qUbiqGeneratorPlugin.cpp \
	generator.cpp \

win32 {
	QMAKE_POST_LINK = "xcopy templates ..\\..\\..\\bin\\templates\\qUbiq /s /e /q /y /i "
}
else {
	QMAKE_POST_LINK = "mkdir ../../../bin/templates/ && cp -r templates ../../../bin/templates/qUbiq/ "
}






