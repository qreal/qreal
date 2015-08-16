# Copyright 2007-2015 QReal Research Group
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

EDITOR_PATH = $$PWD/../$$QREAL_EDITOR_PATH

win32 {
	QRXC_COMMAND = cd $$EDITOR_PATH && $$QRXC
} else:!macx {
	QRXC_COMMAND = export LD_LIBRARY_PATH=$$QRXC_DIR && cd $$EDITOR_PATH && $$QRXC
} else {
	QRXC_COMMAND = export DYLD_LIBRARY_PATH=$$QRXC_DIR && cd $$EDITOR_PATH && $$QRXC
}

qrxc_source.commands = $$QRXC_COMMAND $$QREAL_XML $$ROOT
qrxc_source.depends = $$QRXC $$QREAL_XML_DEPENDS
qrxc_source.input = QREAL_XML
qrxc_source.output = $$EDITOR_PATH/generated/pluginInterface.cpp
qrxc_source.variable_out = SOURCES
qrxc_source.name = QReal_XML_Compiler

QMAKE_EXTRA_COMPILERS += qrxc_source

FAKE_COMMAND = cd .

qrxc_header.commands = $$FAKE_COMMAND
qrxc_header.depends = $$QRXC $$QREAL_XML_DEPENDS $$EDITOR_PATH/generated/pluginInterface.cpp
qrxc_header.input = QREAL_XML
qrxc_header.output = $$EDITOR_PATH/generated/pluginInterface.h
qrxc_header.variable_out = MOC_HEADERS

QMAKE_EXTRA_COMPILERS += qrxc_header

qrxc_elements.commands = $$FAKE_COMMAND
qrxc_elements.depends = $$QRXC $$QREAL_XML_DEPENDS $$EDITOR_PATH/generated/pluginInterface.cpp
qrxc_elements.input = QREAL_XML
qrxc_elements.output = $$EDITOR_PATH/generated/elements.h
qrxc_elements.variable_out = HEADERS

QMAKE_EXTRA_COMPILERS += qrxc_elements

qrxc_resource.commands = $$FAKE_COMMAND
qrxc_resource.depends = $$QRXC $$QREAL_XML_DEPENDS $$EDITOR_PATH/generated/pluginInterface.cpp
qrxc_resource.input = QREAL_XML
qrxc_resource.output = $$EDITOR_PATH/plugin.qrc
qrxc_resource.variable_out = NEW_RESOURCES

QMAKE_EXTRA_COMPILERS += qrxc_resource

# Here we need to call moc explicitly because by default it will be called before any files were generated
new_moc.output  = $$MOC_DIR/moc_${QMAKE_FILE_BASE}.cpp
new_moc.commands = $$dirname(QMAKE_QMAKE)/moc ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
new_moc.input = MOC_HEADERS
new_moc.variable_out = SOURCES

QMAKE_EXTRA_COMPILERS += new_moc

# Here we need to call rcc explicitly because by tefault it will be called before any files were generated
new_rcc.output  = $$RCC_DIR/rcc_${QMAKE_FILE_BASE}.cpp
new_rcc.commands = $$dirname(QMAKE_QMAKE)/rcc ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}
new_rcc.input = NEW_RESOURCES
new_rcc.variable_out = SOURCES

QMAKE_EXTRA_COMPILERS += new_rcc

include(extraCompilersSdf.pri)
