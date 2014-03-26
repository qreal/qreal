QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo -lrobotsGeneratorBase

INCLUDEPATH += \
    $$PWD/../robotsGeneratorBase/ \
    $$PWD/../../../../ \
    $$PWD/../../../../qrgui \

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
    QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../../bin/
    QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../../bin/plugins/
}

TRANSLATIONS = fSharpGenerator_ru.ts

RESOURCES = \
    $$PWD/templates.qrc \

HEADERS += \
    $$PWD/fSharpMasterGenerator.h \
    $$PWD/fSharpGeneratorPlugin.h \
    $$PWD/fSharpGeneratorCustomizer.h \
    $$PWD/fSharpGeneratorFactory.h

SOURCES += \
    $$PWD/fSharpGeneratorCustomizer.cpp \
    $$PWD/fSharpGeneratorPlugin.cpp \
    $$PWD/fSharpMasterGenerator.cpp \
    $$PWD/fSharpGeneratorFactory.cpp


