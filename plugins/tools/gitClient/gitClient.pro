QT += widgets
LIBS += -L../../../bin -lqrkernel -lqrutils -lqrgui-preferences-dialog -lqrgui-versioning-plugins-manager \
			-lqrgui-dialogs

TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
DESTDIR = ../../../bin/plugins/tools/

INCLUDEPATH += \
	$$PWD/ \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui
OBJECTS_DIR = .obj

TRANSLATIONS += gitClient_ru.ts

HEADERS += \
	$$PWD/gitPlugin.h \
	$$PWD/viewInteraction.h \
	$$PWD/tags.h \
	$$PWD/transparentMode.h \
	$$PWD/gui/versioningPage.h \
	$$PWD/gui/commitDialog.h \
	$$PWD/gui/pushDialog.h \
	$$PWD/gui/cloneDialog.h \
	$$PWD/gui/remoteDialog.h \
	$$PWD/gui/pullDialog.h \
	$$PWD/gui/resetDialog.h \
	$$PWD/gui/statusDialog.h \
	$$PWD/gui/changeVersionWidget.h \
	$$PWD/gui/diffBetweenDialog.h \
	$$PWD/gui/branchNameDialog.h \
	$$PWD/gui/simpleOutputDialog.h

SOURCES += \
	$$PWD/gitPlugin.cpp \
	$$PWD/viewInteraction.cpp \
	$$PWD/transparentMode.cpp \
	$$PWD/gui/versioningPage.cpp \
	$$PWD/gui/commitDialog.cpp \
	$$PWD/gui/pushDialog.cpp \
	$$PWD/gui/cloneDialog.cpp \
	$$PWD/gui/remoteDialog.cpp \
	$$PWD/gui/pullDialog.cpp \
	$$PWD/gui/resetDialog.cpp \
	$$PWD/gui/statusDialog.cpp \
	$$PWD/gui/changeVersionWidget.cpp \
	$$PWD/gui/diffBetweenDialog.cpp \
	$$PWD/gui/branchNameDialog.cpp \
	$$PWD/gui/simpleOutputDialog.cpp


FORMS += \
	$$PWD/gui/versioningPage.ui \
	$$PWD/gui/changeVersionWidget.ui
