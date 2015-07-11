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

QT += widgets network

links(qslog qrkernel qrutils)

DEFINES += ROBOTS_UTILS_LIBRARY

HEADERS += \
	$$PWD/include/utils/timelineInterface.h \
	$$PWD/include/utils/abstractTimer.h \
	$$PWD/include/utils/realTimeline.h \
	$$PWD/include/utils/realTimer.h \
	$$PWD/include/utils/tracer.h \
	$$PWD/include/utils/robotCommunication/robotCommunicationThreadInterface.h \
	$$PWD/include/utils/robotCommunication/robotCommunicator.h \
	$$PWD/include/utils/robotCommunication/robotCommunicationException.h \
	$$PWD/include/utils/robotCommunication/tcpRobotCommunicator.h \
	$$PWD/include/utils/robotCommunication/protocol.h \
	$$PWD/include/utils/requiredVersion.h \
	$$PWD/src/robotCommunication/tcpConnectionHandler.h \

SOURCES += \
	$$PWD/src/abstractTimer.cpp \
	$$PWD/src/realTimeline.cpp \
	$$PWD/src/realTimer.cpp \
	$$PWD/src/tracer.cpp \
	$$PWD/src/robotCommunication/robotCommunicator.cpp \
	$$PWD/src/robotCommunication/robotCommunicationException.cpp \
	$$PWD/src/robotCommunication/tcpConnectionHandler.cpp \
	$$PWD/src/robotCommunication/tcpRobotCommunicator.cpp \

TRANSLATIONS += $$PWD/../../../qrtranslations/ru/plugins/robots/robots_utils_ru.ts
