TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
DESTDIR = ../../../bin/plugins/tools/
MOC_DIR = .moc
RCC_DIR = .moc

QT += widgets

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \

LIBS += -L$$PWD/../../../bin -lqrkernel -lqrutils -lqrmc

HEADERS += \
	../../../qrgui/dialogs/preferencesPages/preferencesPage.h \
    generator/databasesGenerator.h \
    generator/datatypesChecker.h \

HEADERS += \
    databasesSupportPlugin.h \
    databasesPreferencesPage.h \
    databasesCustomizer.h

SOURCES += \
	databasesSupportPlugin.cpp \
    databasesPreferencesPage.cpp \
    databasesCustomizer.cpp \
    generator/databasesGenerator.cpp \
    generator/datatypesChecker.cpp \

OTHER_FILES += \
    databasesSupport.pri \
    generator/glossary/datatypes.txt

#include(databasesSupport.pri)

FORMS += \
    databasesPreferencesPage.ui
