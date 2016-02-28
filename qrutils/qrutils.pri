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

HEADERS += \
	$$PWD/outFile.h \
	$$PWD/utilsDeclSpec.h \
	$$PWD/xmlUtils.h \
	$$PWD/watchListWindow.h \
	$$PWD/metamodelGeneratorSupport.h \
	$$PWD/inFile.h \
	$$PWD/scalableItem.h \
	$$PWD/scalableCoordinate.h \
	$$PWD/nameNormalizer.h \
	$$PWD/fileSystemUtils.h \
	$$PWD/stringUtils.h \
	$$PWD/qRealDialog.h \
	$$PWD/qRealFileDialog.h \
	$$PWD/smartDock.h \
	$$PWD/textElider.h\
	$$PWD/virtualKeyboard.h \
	$$PWD/parserErrorReporter.h \
	$$PWD/deleteLaterHelper.h \
	$$PWD/generator/abstractGenerator.h \
	$$PWD/widgetFinder.h \

SOURCES += \
	$$PWD/outFile.cpp \
	$$PWD/xmlUtils.cpp \
	$$PWD/watchListWindow.cpp\
	$$PWD/metamodelGeneratorSupport.cpp \
	$$PWD/inFile.cpp \
	$$PWD/scalableItem.cpp \
	$$PWD/scalableCoordinate.cpp \
	$$PWD/nameNormalizer.cpp \
	$$PWD/fileSystemUtils.cpp \
	$$PWD/stringUtils.cpp \
	$$PWD/qRealDialog.cpp \
	$$PWD/qRealFileDialog.cpp \
	$$PWD/smartDock.cpp \
	$$PWD/textElider.cpp \
	$$PWD/virtualKeyboard.cpp \
	$$PWD/parserErrorReporter.cpp \
	$$PWD/generator/abstractGenerator.cpp \
	$$PWD/widgetFinder.cpp \

FORMS += \
	$$PWD/watchListWindow.ui

RESOURCES = $$PWD/qrutils.qrc

TRANSLATIONS = \
	$$PWD/../qrtranslations/ru/qrutils_ru.ts \
	$$PWD/../qrtranslations/fr/qrutils_fr.ts \

QT += xml widgets

includes(qrtext qrgraph)

links(qrkernel qslog qrtext)

DEFINES += QRUTILS_LIBRARY

# Files for parsing of expressions on diagrams
include($$PWD/expressionsParser/expressionsParser.pri)

# Graphical scene framework used in qrgui and plugins
include($$PWD/graphicsUtils/graphicsUtils.pri)

# Stuff connected with graphs and trees
include($$PWD/graphUtils/graphUtils.pri)

# Math Utils
include($$PWD/mathUtils/mathUtils.pri)

# Real-time plot
include($$PWD/graphicsWatcher/sensorsGraph.pri)

#Plugin managers
include($$PWD/pluginManagers/pluginManagers.pri)

# Tools for interpreting behavioral diagrams
include($$PWD/interpreter/interpreter.pri)

# The collection of useful widgets
include($$PWD/widgets/widgets.pri)
