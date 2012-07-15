TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/plugins/

LIBS += -L../../../bin -lqrkernel -lqrutils

OBJECTS_DIR = .obj
MOC_DIR = .moc

TRANSLATIONS = sbtGeneratorPlugin_ru.ts

RESOURCES = sbtGeneratorPlugin.qrc


HEADERS = \
    sbtGeneratorPlugin.h \
    sbtGenerator.h \
    dataObjectGenerator.h \
    dataIntegratorGenerator.h \
    projectGenerator.h

SOURCES = \
    sbtGeneratorPlugin.cpp \
    sbtGenerator.cpp \
    dataObjectGenerator.cpp \
    dataIntegratorGenerator.cpp \
    projectGenerator.cpp

win32 {
    QMAKE_POST_LINK = "xcopy template ..\\..\\..\\bin\\template /s /e /q /y /i "
}
else {
    QMAKE_POST_LINK = "mkdir ../../../bin/template/ && cp -r template ../../../bin/template/ "
}
