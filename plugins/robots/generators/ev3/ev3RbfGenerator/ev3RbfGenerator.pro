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

TARGET = robots-ev3-rbf-generator

include(../../../../../global.pri)

QT += widgets

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/ev3/ev3GeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-ev3-generator-base)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3RbfGenerator_ru.ts

#QT += widgets

#CONFIG += c++11

#include(../../../../../global.pri)

#TEMPLATE = lib
#CONFIG += plugin

#DESTDIR = $$PWD/../../../../../bin/plugins/tools/kitPlugins/
#TARGET = robots-ev3-rbf-generator

#MOC_DIR = .moc
#RCC_DIR = .moc
#OBJECTS_DIR = .obj

#LIBS += -L$$PWD/../../../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo \
#		-lrobots-generator-base -lrobots-ev3-generator-base \

#INCLUDEPATH += \
#	$$PWD/../ev3GeneratorBase/include/ \
#	$$PWD/../../generatorBase/include/ \
#	$$PWD/../../../interpreters/interpreterBase/include \
#	$$PWD/../generatorBase/src/ \
#	$$PWD/../../../../../ \
#	$$PWD/../../../../../qrgui \
#	$$PWD/../../../../../qrtext/include \

## workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
## when fixed it would become possible to use QMAKE_LFLAGS_RPATH
#!macx {
#	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../../../bin/
#	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../../../bin/plugins/
#}

#TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/ev3RbfGenerator_ru.ts

HEADERS += \
	ev3RbfGeneratorPlugin.h \
	ev3RbfMasterGenerator.h \

SOURCES += \
	ev3RbfGeneratorPlugin.cpp \
	ev3RbfMasterGenerator.cpp \

RESOURCES += \
	ev3RbfGenerator.qrc \
	templates.qrc \
