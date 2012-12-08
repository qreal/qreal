TEMPLATE = lib

DESTDIR = ../bin

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc

DEFINES += QRKERNEL_LIBRARY

HEADERS += \
	$$PWD/ids.h \
	$$PWD/definitions.h \
	$$PWD/exception/exception.h \
	$$PWD/roles.h \
	$$PWD/settingsManager.h \
	$$PWD/kernelDeclSpec.h \
	$$PWD/timeMeasurer.h \

SOURCES += \
	$$PWD/ids.cpp \
	$$PWD/exception/exception.cpp \
	$$PWD/settingsManager.cpp \
	$$PWD/timeMeasurer.cpp \

RESOURCES += \
	$$PWD/qrkernel.qrc


