# Copyright 2016 CyberTech Labs Ltd.
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

QT += widgets network

includes( \
		plugins/robots/trik/generators/trikQtsGeneratorLibrary \
		plugins/robots/trik/generators/trikGeneratorBase \
		plugins/robots/core/generatorBase \
		plugins/robots/core/kitBase \
		plugins/robots/trik/trikCommon \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base \
		robots-kit-base robots-trik-kit robots-utils robots-trik-qts-generator-library \
)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/trikV62QtsGenerator_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/trikV62QtsGenerator_fr.ts \

HEADERS += \
	$$PWD/trikV62QtsGeneratorPlugin.h \

SOURCES += \
	$$PWD/trikV62QtsGeneratorPlugin.cpp \
