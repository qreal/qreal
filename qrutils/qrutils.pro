TEMPLATE = lib

QT += xml

DESTDIR = ../bin

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRUTILS_LIBRARY

HEADERS += \
	outFile.h \
	utilsDeclSpec.h \
	xmlUtils.h \

SOURCES += \
	outFile.cpp \
	xmlUtils.cpp \
