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

TARGET = qrgui-controller

include(../../global.pri)

TEMPLATE = lib

links(qrkernel)
includes(qrgui)

QT += widgets

DEFINES += QRGUI_CONTROLLER_LIBRARY

HEADERS += \
	$$PWD/controllerDeclSpec.h \
	$$PWD/controller.h \
	$$PWD/undoStack.h \
	$$PWD/commands/abstractCommand.h \
	$$PWD/commands/doNothingCommand.h \
	$$PWD/commands/trackingEntity.h \

SOURCES += \
	$$PWD/controller.cpp \
	$$PWD/undoStack.cpp \
	$$PWD/commands/abstractCommand.cpp \
	$$PWD/commands/doNothingCommand.cpp \
	$$PWD/commands/trackingEntity.cpp \
