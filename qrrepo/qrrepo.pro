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
	$$PWD/private/client.h \
	$$PWD/private/qrRepoGlobal.h \
	$$PWD/private/serializer.h \
	$$PWD/private/classes/object.h \
	$$PWD/private/folderCompressor.h \

SOURCES += \
	$$PWD/private/client.cpp \
	$$PWD/private/serializer.cpp \
	$$PWD/private/classes/object.cpp \
	$$PWD/private/folderCompressor.cpp \

# API репозитория
HEADERS += \
	$$PWD/repoApi.h \
	$$PWD/graphicalRepoApi.h \
	$$PWD/logicalRepoApi.h \
	$$PWD/repoControlInterface.h \
	$$PWD/commonRepoApi.h \


SOURCES += \
	$$PWD/private/repoApi.cpp \

OTHER_FILES +=
