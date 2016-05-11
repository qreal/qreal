е# Copyright 2014-2016 Anastasia Semenova
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
DESTDIR = $$DESTDIR/plugins/tools

TRANSLATIONS = \
	$$PWD/../../../qrtranslations/ru/plugins/databases/databasesSupport_ru.ts \

DEPLOYMENT += addFiles

@QT += core sql
QT -= gui@
QT += widgets
QT += sql

INCLUDEPATH += \
	$$PWD/../../.. \
	$$PWD/../../../qrgui/ \
	$$PWD/../../../qrtext/include/ \

links(qrkernel qrutils qrgui-preferences-dialog qrgui-tool-plugin-interface qrgui-models)

HEADERS = \
	$$PWD/generator/databasesGenerator.h \
	$$PWD/databasesSupportPlugin.h \
	$$PWD/databasesPreferencesPage.h \
	$$PWD/databasesCustomizer.h \
	$$PWD/generator/databasesReverseEngineer.h \
	$$PWD/generateSchemaWidget.h

SOURCES = \
	$$PWD/databasesSupportPlugin.cpp \
	$$PWD/databasesPreferencesPage.cpp \
	$$PWD/databasesCustomizer.cpp \
	$$PWD/generator/databasesGenerator.cpp \
	$$PWD/generator/databasesReverseEngineer.cpp \
	$$PWD/generateSchemaWidget.cpp

OTHER_FILES += \
	$$PWD/databasesSupport.pri \
	$$PWD/generator/glossary/datatypes.txt \

FORMS += \
	$$PWD/databasesPreferencesPage.ui \
	$$PWD/generateSchemaWidget.ui


