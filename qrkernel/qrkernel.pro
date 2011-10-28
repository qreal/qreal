TEMPLATE = lib

DESTDIR = ../bin

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRKERNEL_LIBRARY

HEADERS += \
	ids.h \
	definitions.h \
	exception/exception.h \
	roles.h \
	settingsManager.h \
	kernelDeclSpec.h \

SOURCES += \
	ids.cpp \
	exception/exception.cpp \
	settingsManager.cpp \

