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

TEMPLATE = subdirs

include(common.pri)

# editor plugins
SUBDIRS += \
	$$PWD/dragonDiagram/dragonDiagram.pro \
	$$PWD/blockDiagram/blockDiagram.pro \
#	$$PWD/hascol/hascol.pro \
	$$PWD/metaEditor/metaEditor.pro \
	$$PWD/requirements/requirements.pro \
	$$PWD/domainAnalysis/domainAnalysis.pro \
	$$PWD/deployment/deploymentEditor.pro \

# interpreted plugins
SUBDIRS += \
	generationRulesTool/generationRulesTool.pro \

# tool plugins
SUBDIRS += \
	$$PWD/tools/exterminatus/exterminatus.pro \
	$$PWD/tools/visualInterpreter/visualInterpreter.pro \
	$$PWD/tools/refactoring/refactoring.pro \
#	$$PWD/tools/testInvocation/testInvocation.pro \
	$$PWD/tools/uxInfo/uxInfo.pro \


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qrutils/release/ -lqrutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qrutils/debug/ -lqrutils
else:unix: LIBS += -L$$OUT_PWD/../qrutils/ -lqrutils

INCLUDEPATH += $$PWD/../qrutils
DEPENDPATH += $$PWD/../qrutils
