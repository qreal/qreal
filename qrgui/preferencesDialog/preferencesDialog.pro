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

TARGET = qrgui-preferences-dialog

include(../../global.pri)

TEMPLATE = lib

QT += widgets

links(qrkernel qrutils)

DEFINES += QRGUI_PREFERENCES_DIALOG_LIBRARY

TRANSLATIONS = \
	$$PWD/../../qrtranslations/ru/qrgui_preferencesDialog_ru.ts \
	$$PWD/../../qrtranslations/fr/qrgui_preferencesDialog_fr.ts \

HEADERS += \
	$$PWD/preferencesDialogDeclSpec.h \
	$$PWD/preferencesDialog.h \
	$$PWD/preferencesPage.h \
	$$PWD/preferencesPages/behaviourPage.h \
	$$PWD/preferencesPages/debuggerPage.h \
	$$PWD/preferencesPages/editorPage.h \
	$$PWD/preferencesPages/miscellaniousPage.h \
	$$PWD/preferencesPages/featuresPage.h \

SOURCES += \
	$$PWD/preferencesDialog.cpp \
	$$PWD/preferencesPage.cpp \
	$$PWD/preferencesPages/behaviourPage.cpp \
	$$PWD/preferencesPages/debuggerPage.cpp \
	$$PWD/preferencesPages/editorPage.cpp \
	$$PWD/preferencesPages/miscellaniousPage.cpp \
	$$PWD/preferencesPages/featuresPage.cpp \

FORMS += \
	$$PWD/preferencesDialog.ui \
	$$PWD/preferencesPages/miscellaniousPage.ui \
	$$PWD/preferencesPages/editorPage.ui \
	$$PWD/preferencesPages/debuggerPage.ui \
	$$PWD/preferencesPages/behaviourPage.ui \
	$$PWD/preferencesPages/featuresPage.ui \

RESOURCES += \
	$$PWD/preferencesDialog.qrc \
