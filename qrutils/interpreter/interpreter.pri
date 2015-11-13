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
	$$PWD/interpreter.h \
	$$PWD/stopReason.h \
	$$PWD/thread.h \
	$$PWD/blockInterface.h \
	$$PWD/block.h \
	$$PWD/blocksTableInterface.h \
	$$PWD/blocksTableBase.h \
	$$PWD/blocks/emptyBlock.h \
	$$PWD/blocks/initialBlock.h \
	$$PWD/blocks/finalBlock.h \
	$$PWD/blocks/commentBlock.h \
	$$PWD/blocks/ifBlock.h \
	$$PWD/blocks/switchBlock.h \
	$$PWD/blocks/loopBlock.h \
	$$PWD/blocks/subprogramBlock.h \
	$$PWD/blocks/forkBlock.h \
	$$PWD/blocks/functionBlock.h \
	$$PWD/blocks/variableInitBlock.h \
	$$PWD/blocks/unsupportedBlock.h \
	$$PWD/blocks/joinBlock.h \
	$$PWD/blocks/killThreadBlock.h \
	$$PWD/blocks/sendThreadMessageBlock.h \
	$$PWD/blocks/receiveThreadMessageBlock.h \
	$$PWD/blocks/randomInitBlock.h \

SOURCES += \
	$$PWD/interpreter.cpp \
	$$PWD/thread.cpp \
	$$PWD/block.cpp \
	$$PWD/blocksTableBase.cpp \
	$$PWD/blocks/emptyBlock.cpp \
	$$PWD/blocks/initialBlock.cpp \
	$$PWD/blocks/finalBlock.cpp \
	$$PWD/blocks/commentBlock.cpp \
	$$PWD/blocks/ifBlock.cpp \
	$$PWD/blocks/switchBlock.cpp \
	$$PWD/blocks/loopBlock.cpp \
	$$PWD/blocks/subprogramBlock.cpp \
	$$PWD/blocks/forkBlock.cpp \
	$$PWD/blocks/functionBlock.cpp \
	$$PWD/blocks/variableInitBlock.cpp \
	$$PWD/blocks/unsupportedBlock.cpp \
	$$PWD/blocks/randomInitBlock.cpp \
	$$PWD/blocks/joinBlock.cpp \
	$$PWD/blocks/killThreadBlock.cpp \
	$$PWD/blocks/sendThreadMessageBlock.cpp \
	$$PWD/blocks/receiveThreadMessageBlock.cpp \
