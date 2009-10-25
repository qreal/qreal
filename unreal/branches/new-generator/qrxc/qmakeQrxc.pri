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

qrxc_source.depends = ../pluginTemplate.cpp $$QMAKE_QRXC $$QREAL_XML_DEPENDS
qrxc_source.commands = $$QMAKE_QRXC $$QREAL_XML
qrxc_source.input = QREAL_XML
qrxc_source.output = generated/pluginInterface.cpp
qrxc_source.variable_out = SOURCES

qrxc_header.depends = ../pluginTemplate.cpp $$QMAKE_QRXC $$QREAL_XML_DEPENDS
qrxc_header.commands = $$FAKE
qrxc_header.input = QREAL_XML
qrxc_header.output = generated/pluginInterface.h generated/elements.h
qrxc_header.variable_out = HEADERS

qrxc_resource.depends = ../pluginTemplate.cpp $$QMAKE_QRXC $$QREAL_XML_DEPENDS
qrxc_resource.commands = $$FAKE
qrxc_resource.input = QREAL_XML
qrxc_resource.output = generated/shapes/plugin.qrc
qrxc_resource.variable_out = RESOURCES

QMAKE_EXTRA_COMPILERS += qrxc_source qrxc_header qrxc_resource

QRXML_ROOT = ..

!include (../editorsSdk.pri) {
	message("editorsSdk.pri not found at ../")
}

QT += xml

TEMPLATE =  lib
CONFIG += plugin
DESTDIR = ../../qrgui/plugins/
INCLUDEPATH += ../..

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .moc