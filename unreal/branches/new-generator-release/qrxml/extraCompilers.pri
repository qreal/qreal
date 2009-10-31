qrxc_source.commands = $$QRXC $$QREAL_XML
qrxc_source.input = INPUT
qrxc_source.output = generated/pluginInterface.cpp
qrxc_source.variable_out = SOURCES
qrxc_source.name = QReal_XML_Compiler

QMAKE_EXTRA_COMPILERS += qrxc_source

qrxc_header.commands = $$FAKE_COMMAND
qrxc_header.input = INPUT
qrxc_header.output = generated/pluginInterface.h
qrxc_header.variable_out = HEADERS

QMAKE_EXTRA_COMPILERS += qrxc_header

qrxc_resource.commands = $$FAKE_COMMAND
qrxc_resource.input = INPUT
qrxc_resource.output = $$quote(generated/shapes/)$$QREAL_EDITOR_NAME$$quote(.qrc)
qrxc_resource.variable_out = RESOURCES

QMAKE_EXTRA_COMPILERS += qrxc_resource

include(extraCompilersSdf.pri)