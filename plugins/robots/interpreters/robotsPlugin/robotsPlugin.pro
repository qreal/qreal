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

TARGET = robots-plugin

include(../../../../global.pri)

DESTDIR = $$DESTDIR/plugins/tools/

TEMPLATE = lib
CONFIG += plugin

QT += widgets

includes(plugins/robots/interpreters/interpreterCore \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrgui-tool-plugin-interface robots-interpreter-core)

HEADERS += \
	$$PWD/robotsPlugin.h \

SOURCES += \
	$$PWD/robotsPlugin.cpp \

TRANSLATIONS += \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/robotsPlugin_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/robotsPlugin_fr.ts \
