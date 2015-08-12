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

TARGET = 2D-model

include(../../../../global.pri)

TEMPLATE = app

QT += widgets

includes(plugins/robots/interpreters/interpreterCore \
		plugins/robots/common/kitBase \
		plugins/robots/common/twoDModel \
		plugins/robots/utils \
		qrtext \
)

links(qslog qrkernel qrutils qrgui-tool-plugin-interface qrgui-preferences-dialog qrgui-facade \
		qrgui-models qrgui-editor qrgui-plugin-manager qrgui-text-editor \
		robots-utils robots-kit-base robots-interpreter-core robots-2d-model \
)

TRANSLATIONS = \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/twoDModelRunner_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/twoDModelRunner_fr.ts \

HEADERS += \
	$$PWD/runner.h \
	$$PWD/reporter.h \

SOURCES += \
	$$PWD/main.cpp \
	$$PWD/runner.cpp \
	$$PWD/reporter.cpp \
