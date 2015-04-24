include(../../../global.pri)

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qrutils qrgui-preferences-dialog qrgui-versioning-plugins-manager \
		 qrgui-dialogs)

INCLUDEPATH += \
	$$PWD/ \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

TRANSLATIONS += $$PWD/../../../gitClient_ru.ts

QT += widgets

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
