TARGET = qrgui-brand-manager

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

QT += widgets

DEFINES += QRGUI_BRAND_MANAGER_LIBRARY

links(qrkernel qrutils qrgui-plugin-manager)
includes(qrgui)

