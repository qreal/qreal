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

include(../../../global.pri)

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/

includes(thirdparty/qscintilla/Qt4Qt5)

links(qrkernel qslog qrgui-preferences-dialog)

INCLUDEPATH += \
	$$PWD/../../../qrgui/ \

QT += widgets xml

TRANSLATIONS = \
	$$PWD/../../../qrtranslations/ru/plugins/updatesChecker_ru.ts \
	$$PWD/../../../qrtranslations/fr/plugins/updatesChecker_fr.ts \

HEADERS = \
	$$PWD/updatesCheckerPlugin.h \
	$$PWD/updater.h \
	$$PWD/updateVersionDialog.h \

SOURCES = \
	$$PWD/updatesCheckerPlugin.cpp \
	$$PWD/updater.cpp \
	$$PWD/updateVersionDialog.cpp \

RESOURCES += \
	$$PWD/updatesChecker.qrc \
