TEMPLATE = lib

DESTDIR = $$PWD/../../bin

INCLUDEPATH = \
	$$PWD/ \
	$$PWD/../ \
	$$PWD/../../ \

QT += widgets

CONFIG += c++11

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc

HEADERS += \
	$$PWD/hotKeyManagerPage.h \
	$$PWD/hotKeyManager.h \
	$$PWD/shortcutEdit.h \

SOURCES += \
	$$PWD/hotKeyManagerPage.cpp \
	$$PWD/hotKeyManager.cpp \
	$$PWD/shortcutEdit.cpp \

FORMS += \
	$$PWD/hotKeyManagerPage.ui \
