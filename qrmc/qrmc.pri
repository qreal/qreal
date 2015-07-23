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

TARGET = qrmc

include(../global.pri)
include (classes/classes.pri)
include (utils/utils.pri)

QT += xml

CONFIG += c++11

CONFIG += console

DEPENDPATH += \
	. \
	templates \

HEADERS += \
	metaCompiler.h \
	diagram.h \
	editor.h \
	../qrkernel/definitions.h \
	../qrkernel/ids.h \
	../qrrepo/repoApi.h \

SOURCES += \
	metaCompiler.cpp \
	diagram.cpp \
	editor.cpp \
	main.cpp \

links(qrkernel qrutils qrrepo)
includes(qrkernel)

OTHER_FILES += \
	templates/edge.template \
	templates/node.template \
	templates/plugin.qrc.template \
	templates/plugins.pro.template \
	templates/pro.template \
	templates/utils.template \
