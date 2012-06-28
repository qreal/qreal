TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/

LIBS += -L../../../bin -lqrkernel -lqrutils

OBJECTS_DIR = .obj
MOC_DIR = .moc

HEADERS = \
	sbtGeneratorPlugin.h \
    sbtGenerator.h \
    dataObjectGenerator.h \
    dataIntegratorGenerator.h

SOURCES = \
	sbtGeneratorPlugin.cpp \
    sbtGenerator.cpp \
    dataObjectGenerator.cpp \
    dataIntegratorGenerator.cpp

win32 {
	QMAKE_POST_LINK = "xcopy templates ..\\..\\..\\bin\\templates /s /e /q /y /i "
}
else {
	QMAKE_POST_LINK = "mkdir ../../../bin/templates/ && cp -r templates ../../../bin/templates/ "
}
