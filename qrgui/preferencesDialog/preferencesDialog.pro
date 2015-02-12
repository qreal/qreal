TARGET = qrgui-preferences-dialog

include(../../global.pri)

TEMPLATE = lib

TRANSLATIONS = $$PWD/../../qrtranslations/ru/preferencesDialog_ru.ts

QT += widgets

links(qrkernel qrutils)

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
