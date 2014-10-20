TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-preferences-dialog

TRANSLATIONS = $$PWD/../../qrtranslations/ru/preferencesDialog_ru.ts

QT += widgets

CONFIG += c++11

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

LIBS += -L$$PWD/../../bin -lqrkernel -lqrutils

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRGUI_PREFERENCES_DIALOG_LIBRARY

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_preferencesDialog_ru.ts

HEADERS += \
	$$PWD/preferencesDialogDeclSpec.h \
	$$PWD/preferencesDialog.h \
	$$PWD/preferencesPage.h \
	$$PWD/preferencesPages/behaviourPage.h \
	$$PWD/preferencesPages/debuggerPage.h \
	$$PWD/preferencesPages/editorPage.h \
	$$PWD/preferencesPages/miscellaniousPage.h \
	$$PWD/preferencesPages/featuresPage.h \

SOURCES += \
	$$PWD/preferencesDialog.cpp \
	$$PWD/preferencesPage.cpp \
	$$PWD/preferencesPages/behaviourPage.cpp \
	$$PWD/preferencesPages/debuggerPage.cpp \
	$$PWD/preferencesPages/editorPage.cpp \
	$$PWD/preferencesPages/miscellaniousPage.cpp \
	$$PWD/preferencesPages/featuresPage.cpp \

FORMS += \
	$$PWD/preferencesDialog.ui \
	$$PWD/preferencesPages/miscellaniousPage.ui \
	$$PWD/preferencesPages/editorPage.ui \
	$$PWD/preferencesPages/debuggerPage.ui \
	$$PWD/preferencesPages/behaviourPage.ui \
	$$PWD/preferencesPages/featuresPage.ui \

RESOURCES += \
	$$PWD/preferencesDialog.qrc \
