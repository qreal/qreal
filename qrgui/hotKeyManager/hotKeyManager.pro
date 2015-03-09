TARGET = qrgui-hotkey-manager

include(../../global.pri)

TEMPLATE = lib

links(qrgui-preferences-dialog)
includes(qrgui)

QT += widgets

DEFINES += QRGUI_HOTKEY_MANAGER_LIBRARY

TRANSLATIONS = $$PWD/../../qrtranslations/ru/qrgui_hotKeyManager_ru.ts

HEADERS += \
	$$PWD/hotKeyManagerDeclSpec.h \
	$$PWD/hotKeyManagerPage.h \
	$$PWD/hotKeyManager.h \
	$$PWD/shortcutEdit.h \

SOURCES += \
	$$PWD/hotKeyManagerPage.cpp \
	$$PWD/hotKeyManager.cpp \
	$$PWD/shortcutEdit.cpp \

FORMS += \
	$$PWD/hotKeyManagerPage.ui \

RESOURCES += \
	$$PWD/hotKeyManager.qrc \
