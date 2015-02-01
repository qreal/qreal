QT += xml widgets
TEMPLATE =  lib
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

DESTDIR = $$DESTDIR/plugins/editors/

# Needed because somehow QMake fails to create .rcc directory if RESOURCES is in extra compiler output.
RCC_DIR = .build/$$CONFIGURATION/.obj

links(qrkernel)

isEmpty(QREAL_EDITOR_PATH) {
	error(Please set QREAL_EDITOR_PATH variable in a .pro file of your editor as a path to that editor .xml file relative to plugins/)
}

# QMake generates debug and release makefiles and executes this step for debug and release configurations even when exact configuration is specified
# Since there is no point to build debug and release versions of qrxc, we simply skip this step if no qrxc exists.
exists($$QRXC) {
	if (equals(QMAKE_CXX, "g++")) {
		QMAKE_LFLAGS += -Wl,-E
	}

	include(extraCompilers.pri)

	include(editorsSdk.pri)
}
