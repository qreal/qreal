include(../../../global.pri)

TEMPLATE = lib
CONFIG += plugin 
DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qslog qrgui-preferences-dialog qrgui-tool-plugin-interface)

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui/ \

QT += widgets

TRANSLATIONS = $$PWD/../../../qrtranslations/ru/plugins/uxInfo_ru.ts

HEADERS = \
	$$PWD/uxInfoPlugin.h \
	$$PWD/uxInfo.h \
	$$PWD/filterObject.h \

SOURCES = \
	$$PWD/uxInfoPlugin.cpp \
	$$PWD/uxInfo.cpp \
	$$PWD/filterObject.cpp \

RESOURCES += \
	$$PWD/uxInfo.qrc \
