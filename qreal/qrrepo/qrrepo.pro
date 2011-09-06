QT += xml

TARGET = qrrepo

TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

win32 {
		CONFIG(debug, debug|release) {
				DESTDIR = ../qrgui/debug
		} else:CONFIG(release, debug|release){
				DESTDIR = ../qrgui/release
		} else {
				error(Windows build definitely needs to be fixed)
		}
} else {
		DESTDIR = ../qrgui
}

DEFINES += QRREPO_LIBRARY

# API репозитория
HEADERS += \
	repoApi.h \
	graphicalRepoApi.h \
	logicalRepoApi.h \
	repoControlInterface.h \
	commonRepoApi.h \
	versionControlSystemInterface.h \
	private/client.h \
	private/qrRepoGlobal.h \
	private/serializer.h \
	private/classes/object.h \
	private/externalClient.h \
	private/serializerVersioningInterface.h


SOURCES += \
	private/repoApi.cpp \
	private/client.cpp \
	private/serializer.cpp \
	private/classes/object.cpp \
	private/externalClient.cpp

#svn support
INCLUDEPATH += include

# Код из qrgui/kernel, который тоже неплохо бы куда-нибудь вынести
include(qrguiCode.pri)

# .pri заведён, чтобы структура папок более круто показывалась в креаторе.
# Содержит код, общий для генератора редакторов и основной части.
include (../utils/utils.pri)
