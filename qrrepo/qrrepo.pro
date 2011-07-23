QT += xml

DESTDIR = ../bin

TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

LIBS += -L../bin -lqrkernel

DEFINES += QRREPO_LIBRARY

HEADERS += \
	private/client.h \
	private/qrRepoGlobal.h \
	private/serializer.h \
	private/classes/object.h

SOURCES += \
	private/client.cpp \
	private/serializer.cpp \
	private/classes/object.cpp

# API репозитория
HEADERS += \
	repoApi.h \
	graphicalRepoApi.h \
	logicalRepoApi.h \
	repoControlInterface.h \
	commonRepoApi.h \


SOURCES += \
	private/repoApi.cpp \

#svn support
INCLUDEPATH += include

# .pri заведён, чтобы структура папок более круто показывалась в креаторе.
# Содержит код, общий для генератора редакторов и основной части.
include (../utils/utils.pri)
