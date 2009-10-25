win32 {
        CONFIG(debug, debug|release) {
                QMAKE_QRXC = $$PWD/debug/qrxc.exe
        } else:CONFIG(release, debug|release){
                QMAKE_QRXC = $$PWD/release/qrxc.exe
        } else {
                error(Windows build definitely needs to be fixed)
        }
} else {
        QMAKE_QRXC = $$PWD/qrxc
}

CONFIG(debug, debug|release) {
	QR_PLUGIN_CONFIG = CONFIG+=debug
} else:CONFIG(release, debug|release){
	QR_PLUGIN_CONFIG = CONFIG+=release
} else {
	error(CONFIG is wrong, can not determine build configuration for plugins)
}

qrxc_source.commands = $$QMAKE_QRXC $$QREAL_XML generated/${QMAKE_FILE_IN_BASE}.pro
qrxc_source.input = QREAL_XML
qrxc_source.output = generated/pluginInterface.cpp
qrxc_source.variable_out = SOURCES

QMAKE_EXTRA_COMPILERS += qrxc_source

qrxc_h.commands = $$FAKE
qrxc_h.input = QREAL_XML
qrxc_h.output = generated/pluginInterface.h
qrxc_h.variable_out = HEADERS

QMAKE_EXTRA_COMPILERS += qrxc_h

qrxc_r.commands = $$FAKE
qrxc_r.input = QREAL_XML
qrxc_r.output = generated/shapes/${QMAKE_FILE_IN_BASE}.qrc
qrxc_r.variable_out = RESOURCES

QMAKE_EXTRA_COMPILERS += qrxc_r

QRXML_ROOT = ..

!include (../editorsSdk.pri) {
	message("editorsSdk.pri not found at ../")
}

QT += xml

TEMPLATE =  lib
CONFIG += plugin
DESTDIR = ../../qrgui/plugins/
INCLUDEPATH += ../

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc