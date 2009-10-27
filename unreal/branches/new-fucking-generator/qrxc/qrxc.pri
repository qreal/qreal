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

qrxc_moc.commands = moc generated/pluginInterface.h -o .moc/moc_pluginInterface.cpp
qrxc_moc.input = QREAL_XML
qrxc_moc.output = .moc/moc_pluginInterface.cpp
qrxc_moc.variable_out = SOURCES

QMAKE_EXTRA_COMPILERS += qrxc_moc

qrxc_rcc.commands = rcc -name ${QMAKE_FILE_IN_BASE} generated/shapes/${QMAKE_FILE_IN_BASE}.qrc -o .moc/qrc_${QMAKE_FILE_IN_BASE}.cpp
qrxc_rcc.input = QREAL_XML
qrxc_rcc.output = .moc/qrc_${QMAKE_FILE_IN_BASE}.cpp
qrxc_rcc.variable_out = SOURCES

QMAKE_EXTRA_COMPILERS += qrxc_rcc


# чтобы файл было видно в IDE
qrxc_fake.commands = $$FAKE
qrxc_fake.input = QREAL_XML
qrxc_fake.output = generated/pluginInterface.h
qrxc_fake.variable_out = SOURCES

QMAKE_EXTRA_COMPILERS += qrxc_fake

#чтобы не было предупреждения о том, что файл указан несколько раз (.h в sources нельзя)
QMAKE_LFLAGS += /ignore:4042

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