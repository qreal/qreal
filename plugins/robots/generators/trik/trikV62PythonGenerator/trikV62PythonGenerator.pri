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
		plugins/robots/generators/trik/trikPythonGeneratorLibrary \
		plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/trikKit \
		plugins/robots/utils \
		qrtext \
		thirdparty/qscintilla/Qt4Qt5 \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base \
		robots-kit-base robots-trik-kit robots-utils robots-trik-python-generator-library \
)

TRANSLATIONS = \
	$$PWD/../../../../../qrtranslations/ru/plugins/robots/trikV62PythonGenerator_ru.ts \
	$$PWD/../../../../../qrtranslations/fr/plugins/robots/trikV62PythonGenerator_fr.ts \

HEADERS += \
	$$PWD/trikV62PythonGeneratorPlugin.h \

SOURCES += \
	$$PWD/trikV62PythonGeneratorPlugin.cpp \
