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

