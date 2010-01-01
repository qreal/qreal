QT += xml

TARGET = qrrepo

TEMPLATE = lib

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

HEADERS += \
	private/client.h \
	private/classes/logicObject.h \
	private/qrRepoGlobal.h \

SOURCES += \
	private/client.cpp \
	private/classes/logicObject.cpp \

# API репозитория
HEADERS += repoApi.h \

SOURCES += private/repoApi.cpp \

# Код из qrgui/kernel, который тоже неплохо бы куда-нибудь вынести
include(qrguiCode.pri)
