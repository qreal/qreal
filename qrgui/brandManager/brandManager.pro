TEMPLATE = lib

DESTDIR = $$PWD/../../bin

TARGET = qrgui-brand-manager

LIBS += -L$$PWD/../../bin -lqrkernel -lqrutils -lqrgui-plugin-manager

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

DEFINES += QRGUI_BRAND_MANAGER_LIBRARY

HEADERS += \
	$$PWD/brandManagerDeclSpec.h \
	$$PWD/brandManager.h \
	$$PWD/fonts.h \
	$$PWD/styles.h \

SOURCES += \
	$$PWD/brandManager.cpp \

RESOURCES += \
	$$PWD/fonts/fonts.qrc \
	$$PWD/styles/styles.qrc \
