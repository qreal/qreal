TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
DESTDIR = ../../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

LIBS += -L../../../bin -lqrkernel -lqrutils

HEADERS += \
    databasesSupportPlugin.h \
    databasesPreferencesPage.h \
    databasesCustomizer.h

SOURCES += \
	databasesSupportPlugin.cpp \
    databasesPreferencesPage.cpp \
    databasesCustomizer.cpp

OTHER_FILES += \
    databasesSupport.pri

include(databasesSupport.pri)
