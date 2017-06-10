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

QT += widgets

includes(plugins/robots/common/pioneerKit \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qrutils qrtext qrgui-preferences-dialog qrgui-tool-plugin-interface \
		robots-utils robots-kit-base qslog \
)

SOURCES += $$files(*.cpp, true)
HEADERS += $$files(*.h, true)

TRANSLATIONS += \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/pioneerKit_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/pioneerKit_fr.ts \
