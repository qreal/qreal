QT += widgets
LIBS += -L../../../bin -lqrkernel -lqrutils -lqrgui-preferences-dialog -lqrgui-versioning-plugins-manager

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
DESTDIR = ../../../bin/plugins/tools/

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui
OBJECTS_DIR = .obj

TRANSLATIONS += gitClient_ru.ts

HEADERS += \
    gitPlugin.h \
    viewInteraction.h \
    tags.h \
	transparentMode.h \
    gui/versioningPage.h \
#    ../../qrgui/dialogs/preferencesPages/preferencesPage.h \
    gui/commitDialog.h \
    gui/pushDialog.h \
    gui/cloneDialog.h \
    gui/remoteDialog.h \
    gui/pullDialog.h \
    gui/resetDialog.h \
    gui/statusDialog.h \
    gui/changeVersionWidget.h \
    gui/diffBetweenDialog.h \
    gui/branchNameDialog.h \
    gui/simpleOutputDialog.h

SOURCES += \
    gitPlugin.cpp \
    viewInteraction.cpp \
	transparentMode.cpp \
    gui/versioningPage.cpp \
    gui/commitDialog.cpp \
    gui/pushDialog.cpp \
    gui/cloneDialog.cpp \
    gui/remoteDialog.cpp \
    gui/pullDialog.cpp \
    gui/resetDialog.cpp \
    gui/statusDialog.cpp \
    gui/changeVersionWidget.cpp \
    gui/diffBetweenDialog.cpp \
    gui/branchNameDialog.cpp \
    gui/simpleOutputDialog.cpp


FORMS += \
	gui/versioningPage.ui \
	gui/changeVersionWidget.ui
