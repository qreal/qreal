QT += widgets
LIBS += -L../../bin -lqrkernel -lqrutils

CONFIG += c++11

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/

MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui
OBJECTS_DIR = .obj

TRANSLATIONS += subversionClient_ru.ts

HEADERS += \
    gitPlugin.h \
    viewInteraction.h \
    tags.h \
    gui/versioningPage.h \
    ../../qrgui/dialogs/preferencesPages/preferencesPage.h \
    gui/commitDialog.h \
    gui/pushDialog.h \
    gui/cloneDialog.h \
    gui/remoteDialog.h \
    gui/pullDialog.h \
    gui/resetDialog.h \
    gui/logDialog.h \
    gui/statusDialog.h \
    gui/remoteListDialog.h

SOURCES += \
    gitPlugin.cpp \
    viewInteraction.cpp \
    gui/versioningPage.cpp \
    gui/commitDialog.cpp \
    gui/pushDialog.cpp \
    gui/cloneDialog.cpp \
    gui/remoteDialog.cpp \
    gui/pullDialog.cpp \
    gui/resetDialog.cpp \
    gui/logDialog.cpp \
    gui/statusDialog.cpp \
    gui/remoteListDialog.cpp

FORMS += \
    gui/versioningPage.ui
