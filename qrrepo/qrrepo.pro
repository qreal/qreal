QT += xml

DESTDIR = ../bin

TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

LIBS += -L../bin -lqrkernel -lqrutils

!macx {
	QMAKE_LFLAGS="-Wl,-O1,-rpath,$(PWD)"
}

DEFINES += QRREPO_LIBRARY

HEADERS += \
	private/client.h \
	private/qrRepoGlobal.h \
	private/serializer.h \
	private/classes/object.h \
	private/folderCompressor.h

SOURCES += \
	private/client.cpp \
	private/serializer.cpp \
	private/classes/object.cpp \
	private/folderCompressor.cpp

# API репозитория
HEADERS += \
	repoApi.h \
	graphicalRepoApi.h \
	logicalRepoApi.h \
	repoControlInterface.h \
	commonRepoApi.h \


SOURCES += \
	private/repoApi.cpp \

OTHER_FILES +=











