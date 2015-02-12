include(../../global.pri)

TEMPLATE = lib
CONFIG += plugin 
DESTDIR = $$DESTDIR/plugins/interpreted/

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \

QT += widgets

HEADERS = \
	generationRulesPlugin.h \

SOURCES = \
	generationRulesPlugin.cpp \
