# Copyright 2007-2017 QReal Research Group
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

INCLUDEPATH += \
	$$PWD/../../../.. \
	$$PWD/../../../../qrgui/ \

links(qrkernel qrutils qrrepo qrgui-preferences-dialog)

HEADERS = \
	$$PWD/umlCheckerPlugin.h \
	$$PWD/umlCheckerTmplWindow.h \
	$$PWD/umlCheckerPreferencePage.h \
	$$PWD/umlCheckerTemplate.h \
	$$PWD/umlCheckerPerfectSolution.h \
	$$PWD/umlCheckerHandler.h


SOURCES = \
	$$PWD/umlCheckerPlugin.cpp \
	$$PWD/umlCheckerTmplWindow.cpp \
	$$PWD/umlCheckerPreferencePage.cpp \
	$$PWD/umlCheckerTemplate.cpp \
	$$PWD/umlCheckerPerfectSolution.cpp \
	$$PWD/umlCheckerHandler.cpp


FORMS += \
	$$PWD/umlCheckerTmplWindow.ui \
	$$PWD/umlCheckerPreferencePage.ui
