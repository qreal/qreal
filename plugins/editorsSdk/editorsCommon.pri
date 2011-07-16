QT += xml
TEMPLATE =  lib
CONFIG += plugin

isEmpty(ROOT) {
	error(Please set ROO variable in a .pro file of your editor as a path to a root folder of QReal sources)
}

DESTDIR = $$ROOT/qrgui/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

win32 | win32-msvc* {
	CONFIG(debug, debug|release) {
		QRXC = $$ROOT\\qrxc\\debug\\qrxc.exe
	} else:CONFIG(release, debug|release){
		QRXC = $$ROOT\\qrxc\\release\\qrxc.exe
	} else {
		error(Windows build definitely needs to be fixed)
	}
} else {
	QRXC = $$ROOT/qrxc/qrxc
}

isEmpty(QREAL_EDITOR_PATH) {
	error(Please set QREAL_EDITOR_PATH variable in a .pro file of your editor as a path to that editor .xml file relative to /plugins/)
}

!exists(..\\$$QREAL_EDITOR_PATH/generated/pluginInterface.h) {
	COMMAND = cd ..\\$$QREAL_EDITOR_PATH && \"$$QRXC\" $$QREAL_XML $$ROOT
	SYS = $$system($$COMMAND)
}

if (equals(QMAKE_CXX, "g++")) {
	QMAKE_LFLAGS += -Wl,-E
}

include(extraCompilers.pri)

include(editorsSdk.pri)
