QT += xml widgets
TEMPLATE = lib
CONFIG += plugin

include($$PWD/../../global.pri)

isEmpty(ROOT) {
	error(Please set ROOT variable in a .pro file of your editor as a path to a root folder of QReal sources)
}

win32 {
	QRXC = $$DESTDIR/qrxc$${CONFIGURATION_SUFFIX}.exe
} else {
	QRXC = $$DESTDIR/qrxc$$CONFIGURATION_SUFFIX
}

QRXC_DIR = $$DESTDIR
DESTDIR = $$DESTDIR/plugins/editors/

# Needed because somehow QMake fails to create .rcc directory if RESOURCES is in extra compiler output.
RCC_DIR = .build/$$CONFIGURATION/obj

links(qrkernel)

isEmpty(QREAL_EDITOR_PATH) {
	error(Please set QREAL_EDITOR_PATH variable in a .pro file of your editor as a path to that editor .xml file relative to plugins/)
}

if (equals(QMAKE_CXX, "g++")) {
	QMAKE_LFLAGS += -Wl,-E
}

include(extraCompilers.pri)

include(editorsSdk.pri)
