DESTDIR = ../bin/qrUpdater

CONFIG += c++11

QT       += core network

CONFIG += console

TARGET = qrUpdater
TEMPLATE = app

HEADERS  += \
	$$PWD/src/detailsParser.h \
	$$PWD/src/xmlDataParser.h \
	$$PWD/src/downloader.h \
	$$PWD/src/communicator.h \
	$$PWD/src/update.h \
	$$PWD/src/updateProcessor.h \
	$$PWD/src/updateStorage.h \
	$$PWD/src/argsParser.h \
	$$PWD/src/updatesInstaller.h \

SOURCES += src/main.cpp\
	$$PWD/src/detailsParser.cpp \
	$$PWD/src/xmlDataParser.cpp \
	$$PWD/src/downloader.cpp \
	$$PWD/src/communicator.cpp \
	$$PWD/src/update.cpp \
	$$PWD/src/updateProcessor.cpp \
	$$PWD/src/updateStorage.cpp \
	$$PWD/src/argsParser.cpp \
	$$PWD/src/updatesInstaller.cpp

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc
