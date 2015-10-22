#-------------------------------------------------
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

include(../../../../global.pri)

TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/common/trikKit \
		plugins/robots/common/twoDModel \
		plugins/robots/common/kitBase \
		plugins/robots/interpreters/trikKitInterpreterCommon \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base robots-2d-model robots-trik-kit qextserialport qslog robots-trik-kit-interpreter-common \
)

INCLUDEPATH += \
	$$PWD/../../../../ \
	$$PWD/../../../../qrgui/ \
	plugins/robots/common/kitBase \

QT += widgets

HEADERS = \
	testbuttonplugin.h \

SOURCES = \
	testbuttonplugin.cpp \
