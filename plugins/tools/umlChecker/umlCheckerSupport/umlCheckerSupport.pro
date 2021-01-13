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

include(../../../../global.pri)
QT += widgets

QT += xml

TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$DESTDIR/plugins/tools/

links(qrkernel qrutils qrrepo qrgui-preferences-dialog)


INCLUDEPATH += \
	$$PWD/../../../.. \
	$$PWD/../../../../qrgui/ \


HEADERS = \
	umlCheckerPlugin.h \
	umlCheckerHandler.h \
	umlCheckerTmplWindow.h \
	umlCheckerTemplate.h \
	umlCheckerPerfectSolution.h \
	umlCheckerCustomizer.h


SOURCES = \
	umlCheckerPlugin.cpp \
	umlCheckerHandler.cpp \
	umlCheckerTmplWindow.cpp \
	umlCheckerTemplate.cpp \
	umlCheckerPerfectSolution.cpp \
	umlCheckerCustomizer.cpp

FORMS += \
	umlCheckerTmplWindow.ui

win32 {
	system(cmd /C "xcopy test $$DESTDIR\\templates\\ /s /e /y")
	system(cmd /C "xcopy edge $$DESTDIR\\edge\\ /s /e /y")
}
else {
	system(mkdir -p $$DESTDIR/templates/; mkdir -p $$DESTDIR/edge/; cp test/* $$DESTDIR/templates/; cp edge/* $$DESTDIR/edge/)
	system(mkdir -p $$DESTDIR/../../translations/; mkdir -p $$DESTDIR/../../translations/fr; mkdir -p $$DESTDIR/../../translations/ru;)
	system(touch $$DESTDIR/../../changelog.txt; touch $$DESTDIR/../editors/changelog.txt)


}
