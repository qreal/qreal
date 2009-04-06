QT		+= xml
TEMPLATE        =  lib
CONFIG          += plugin
DESTDIR		=  ../../qrgui/plugins
INCLUDEPATH	+= ..

SOURCES		= \
			editor_handmade.cpp

HEADERS		= \
			editor_handmade.h \
			kroki.h

RESOURCES	=  shapes/editor_handmade.qrc

include(../editors_sdk.pri)