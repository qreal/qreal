DESTDIR = ../bin/qrUpdater

QT       += core network

CONFIG += console

TARGET = qrUpdater
TEMPLATE = app


SOURCES += src/main.cpp\
    src/detailsParser.cpp \
    src/xmlDataParser.cpp \
    src/downloader.cpp \
    src/communicator.cpp \
    src/update.cpp \
    src/updateProcessor.cpp \
    src/updateStorage.cpp \
    src/argsParser.cpp \
    src/updatesInstaller.cpp

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .moc


HEADERS  += \
    src/detailsParser.h \
    src/xmlDataParser.h \
    src/downloader.h \
    src/communicator.h \
    src/update.h \
    src/updateProcessor.h \
    src/updateStorage.h \
    src/argsParser.h \
    src/updatesInstaller.h

