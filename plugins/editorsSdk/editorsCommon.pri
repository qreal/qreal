QT += xml
TEMPLATE =  lib
CONFIG += plugin

isEmpty(ROOT) {
	error(Please set ROOT variable in a .pro file of your editor as a path to a root folder of QReal sources)
}

DESTDIR = $$ROOT/bin/plugins/
MOC_DIR = .moc
RCC_DIR = .moc

win32 {
	QRXC = $$ROOT/bin/qrxc.exe
} else {
	QRXC = $$ROOT/bin/qrxc
}

LIBS += -L$$ROOT/bin -lqrkernel

!macx {
	#QMAKE_LFLAGS += "-Wl,-O1,-rpath,$(PWD)/../../../bin/"

	QMAKE_LFLAGS += "-Wl,-O1,-rpath,."
}

isEmpty(QREAL_EDITOR_PATH) {
	error(Please set QREAL_EDITOR_PATH variable in a .pro file of your editor as a path to that editor .xml file relative to /plugins/)
}

win32 {
	!exists(..\\$$QREAL_EDITOR_PATH/generated/pluginInterface.h) {
		COMMAND = cd ..\\$$QREAL_EDITOR_PATH && \"$$QRXC\" $$QREAL_XML $$ROOT
		SYS = $$system($$COMMAND)
	}
} else {
	!exists(../$$QREAL_EDITOR_PATH/generated/pluginInterface.h) {
		COMMAND = cd ../$$QREAL_EDITOR_PATH && \"$$QRXC\" $$QREAL_XML $$ROOT
		SYS = $$system($$COMMAND)
	}
}


if (equals(QMAKE_CXX, "g++")) {
	QMAKE_LFLAGS += -Wl,-E
}

include(extraCompilers.pri)

include(editorsSdk.pri)
