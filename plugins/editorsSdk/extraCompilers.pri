qrxc_source.commands = $$QRXC $$QREAL_XML $$ROOT
qrxc_source.depends = $$QRXC $$QREAL_XML_DEPENDS
qrxc_source.input = QREAL_XML
qrxc_source.output = generated/pluginInterface.cpp
qrxc_source.variable_out = SOURCES
qrxc_source.name = QReal_XML_Compiler

QMAKE_EXTRA_COMPILERS += qrxc_source

FAKE_COMMAND = cd .

qrxc_header.commands = $$FAKE_COMMAND
qrxc_header.depends = $$QRXC $$QREAL_XML_DEPENDS
qrxc_header.input = QREAL_XML
qrxc_header.output = generated/pluginInterface.h
qrxc_header.variable_out = HEADERS

QMAKE_EXTRA_COMPILERS += qrxc_header

qrxc_elements.commands = $$FAKE_COMMAND
qrxc_elements.depends = $$QRXC $$QREAL_XML_DEPENDS
qrxc_elements.input = QREAL_XML
qrxc_elements.output = generated/elements.h
qrxc_elements.variable_out = HEADERS

QMAKE_EXTRA_COMPILERS += qrxc_elements

qrxc_resource.commands = $$FAKE_COMMAND
qrxc_resource.depends = $$QRXC $$QREAL_XML_DEPENDS
qrxc_resource.input = QREAL_XML
qrxc_resource.output = plugin.qrc
qrxc_resource.variable_out = RESOURCES

QMAKE_EXTRA_COMPILERS += qrxc_resource

include(extraCompilersSdf.pri)
