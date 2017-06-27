# Copyright 2017 QReal Research Group
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

TARGET = robots-pioneer-lua-generator

include(../../../../../global.pri)

!win32 {
	copyToDestdir(scripts/pioneerUpload.sh, now)
	copyToDestdir(scripts/pioneerStart.sh, now)
	copyToDestdir(scripts/pioneerCompile.sh, now)
} else {
	copyToDestdir(scripts/pioneerUpload.bat, now)
	copyToDestdir(scripts/pioneerStart.bat, now)
	copyToDestdir(scripts/pioneerCompile.bat, now)
}

copyToDestdir(thirdparty/controller/controller.exe, now)
copyToDestdir(thirdparty/controller/controller, now)
copyToDestdir(thirdparty/luac, now)

!win32 {
	system("chmod +x $$DESTDIR/pioneerUpload.sh")
	system("chmod +x $$DESTDIR/pioneerStart.sh")
	system("chmod +x $$DESTDIR/pioneerCompile.sh")
	system("chmod +x $$DESTDIR/controller")
}

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes( \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/common/pioneerKit \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 qrgui-text-editor qrgui-preferences-dialog qrtext robots-kit-base \
		robots-generator-base robots-pioneer-kit robots-utils)

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/pioneerLuaGenerator_ru.ts

HEADERS += \
	$$PWD/pioneerLuaGeneratorCustomizer.h \
	$$PWD/pioneerLuaGeneratorFactory.h \
	$$PWD/pioneerLuaGeneratorPlugin.h \
	$$PWD/pioneerLuaMasterGenerator.h \
	$$PWD/communicator/controllerCommunicator.h \
	$$PWD/communicator/httpCommunicator.h \
	$$PWD/communicator/communicatorInterface.h \
	$$PWD/generators/pioneerStateMachineGenerator.h \
	$$PWD/robotModel/pioneerGeneratorRobotModel.h \
	$$PWD/simpleGenerators/geoLandingGenerator.h \
	$$PWD/simpleGenerators/geoTakeoffGenerator.h \
	$$PWD/simpleGenerators/goToPointGenerator.h \
	$$PWD/simpleGenerators/initialNodeGenerator.h \
	$$PWD/simpleGenerators/pioneerPrintGenerator.h \
	$$PWD/simpleGenerators/pioneerSystemGenerator.h \
	$$PWD/widgets/pioneerAdditionalPreferences.h \

SOURCES += \
	$$PWD/pioneerLuaGeneratorCustomizer.cpp \
	$$PWD/pioneerLuaGeneratorFactory.cpp \
	$$PWD/pioneerLuaGeneratorPlugin.cpp \
	$$PWD/pioneerLuaMasterGenerator.cpp \
	$$PWD/communicator/controllerCommunicator.cpp \
	$$PWD/communicator/httpCommunicator.cpp \
	$$PWD/generators/pioneerStateMachineGenerator.cpp \
	$$PWD/robotModel/pioneerGeneratorRobotModel.cpp \
	$$PWD/simpleGenerators/geoLandingGenerator.cpp \
	$$PWD/simpleGenerators/geoTakeoffGenerator.cpp \
	$$PWD/simpleGenerators/goToPointGenerator.cpp \
	$$PWD/simpleGenerators/initialNodeGenerator.cpp \
	$$PWD/simpleGenerators/pioneerPrintGenerator.cpp \
	$$PWD/simpleGenerators/pioneerSystemGenerator.cpp \
	$$PWD/widgets/pioneerAdditionalPreferences.cpp \

FORMS += \
	$$PWD/widgets/pioneerAdditionalPreferences.ui \

RESOURCES += \
	$$PWD/pioneerLuaGenerator.qrc \
	$$PWD/templates.qrc \

OTHER_FILES += \
	$$PWD/scripts/pioneerStart.sh \
	$$PWD/scripts/pioneerUpload.sh \
	$$PWD/scripts/pioneerCompile.sh \
	$$PWD/scripts/pioneerStart.bat \
	$$PWD/scripts/pioneerUpload.bat \
	$$PWD/scripts/pioneerCompile.bat \
	$$PWD/pioneerLuaDefaultSettings.ini \
