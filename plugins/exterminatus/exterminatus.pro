TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

INCLUDEPATH += \
	$$PWD/../.. \
	$$PWD/../../qrgui/ \

QT += widgets

HEADERS = \
	exterminatusPlugin.h \

SOURCES = \
	exterminatusPlugin.cpp \
