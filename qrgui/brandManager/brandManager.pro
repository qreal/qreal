include(../../global.pri)

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

TEMPLATE = lib

TARGET = qrgui-brand-manager

QT += widgets

DEFINES += QRGUI_BRAND_MANAGER_LIBRARY

uses(qrkernel qrutils qrgui-plugin-manager)

