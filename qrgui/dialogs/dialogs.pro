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

TARGET = qrgui-dialogs

include(../../global.pri)

TEMPLATE = lib

QT += widgets

links(qrkernel qrutils qrgui-models)
includes(qrgui)

DEFINES += QRGUI_DIALOGS_LIBRARY

TRANSLATIONS = \
	$$PWD/../../qrtranslations/ru/qrgui_dialogs_ru.ts \
	$$PWD/../../qrtranslations/fr/qrgui_dialogs_fr.ts \

HEADERS += \
	$$PWD/dialogsDeclSpec.h \
	$$PWD/metamodelingOnFly/propertiesDialog.h \
	$$PWD/metamodelingOnFly/editPropertiesDialog.h \
	$$PWD/metamodelingOnFly/chooseTypeDialog.h \
	$$PWD/metamodelingOnFly/edgePropertiesDialog.h \
	$$PWD/metamodelingOnFly/addNodeDialog.h \
	$$PWD/metamodelingOnFly/restorePropertiesDialog.h \
	$$PWD/metamodelingOnFly/restoreElementDialog.h \
	$$PWD/progressDialog/progressBar.h \
	$$PWD/progressDialog/progressDialog.h \
	$$PWD/projectManagement/listWidget.h \
	$$PWD/projectManagement/managedClosableDialog.h \
	$$PWD/projectManagement/suggestToCreateDiagramWidget.h \
	$$PWD/projectManagement/suggestToCreateDiagramDialog.h \
	$$PWD/projectManagement/suggestToCreateProjectDialog.h \
	$$PWD/findReplaceDialog.h \

SOURCES += \
	$$PWD/metamodelingOnFly/propertiesDialog.cpp \
	$$PWD/metamodelingOnFly/editPropertiesDialog.cpp \
	$$PWD/metamodelingOnFly/chooseTypeDialog.cpp \
	$$PWD/metamodelingOnFly/edgePropertiesDialog.cpp \
	$$PWD/metamodelingOnFly/addNodeDialog.cpp \
	$$PWD/metamodelingOnFly/restorePropertiesDialog.cpp \
	$$PWD/metamodelingOnFly/restoreElementDialog.cpp \
	$$PWD/progressDialog/progressBar.cpp \
	$$PWD/progressDialog/progressDialog.cpp \
	$$PWD/projectManagement/listWidget.cpp \
	$$PWD/projectManagement/managedClosableDialog.cpp \
	$$PWD/projectManagement/suggestToCreateDiagramWidget.cpp \
	$$PWD/projectManagement/suggestToCreateDiagramDialog.cpp \
	$$PWD/projectManagement/suggestToCreateProjectDialog.cpp \
	$$PWD/findReplaceDialog.cpp \

FORMS += \
	$$PWD/metamodelingOnFly/propertiesDialog.ui \
	$$PWD/metamodelingOnFly/editPropertiesDialog.ui \
	$$PWD/metamodelingOnFly/chooseTypeDialog.ui \
	$$PWD/metamodelingOnFly/edgePropertiesDialog.ui \
	$$PWD/metamodelingOnFly/addNodeDialog.ui \
	$$PWD/metamodelingOnFly/restorePropertiesDialog.ui \
	$$PWD/metamodelingOnFly/restoreElementDialog.ui \
	$$PWD/findReplaceDialog.ui \
