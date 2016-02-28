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

TARGET = qrgui-mouse-gestures

include(../../global.pri)

TEMPLATE = lib

links(qrkernel qrutils)
includes(qrgui qrgraph)

QT += widgets

DEFINES += QRGUI_MOUSE_GESTURES_LIBRARY

TRANSLATIONS = \
	$$PWD/../../qrtranslations/ru/qrgui_mouseGestures_ru.ts \
	$$PWD/../../qrtranslations/fr/qrgui_mouseGestures_fr.ts \

HEADERS += \
	$$PWD/mouseGesturesDeclSpec.h \
	$$PWD/mouseMovementManagerInterface.h \
	$$PWD/mouseMovementManager.h \
	$$PWD/gesturesWidget.h \
	$$PWD/gesturePainter.h \
	$$PWD/private/geometricForms.h \
	$$PWD/private/pathCorrector.h \
	$$PWD/private/levenshteinDistance.h \
	$$PWD/private/keyManager.h \
	$$PWD/private/keyBuilder.h \
	$$PWD/private/abstractRecognizer.h \
	$$PWD/private/mixedgesturesmanager.h \
	$$PWD/private/rectanglegesturesmanager.h \
	$$PWD/private/nearestposgridgesturesmanager.h \
	$$PWD/private/sorts.h \
	$$PWD/dummyMouseMovementManager.h

SOURCES += \
	$$PWD/mouseMovementManager.cpp \
	$$PWD/gesturesWidget.cpp \
	$$PWD/gesturePainter.cpp \
	$$PWD/private/pathCorrector.cpp \
	$$PWD/private/levenshteinDistance.cpp \
	$$PWD/private/keyManager.cpp \
	$$PWD/private/keyBuilder.cpp \
	$$PWD/private/mixedgesturesmanager.cpp \
	$$PWD/private/rectanglegesturesmanager.cpp \
	$$PWD/private/nearestposgridgesturesmanager.cpp \
	$$PWD/dummyMouseMovementManager.cpp

FORMS += \
	$$PWD/gesturesWidget.ui \
