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
	$$PWD/../include/utils/timelineInterface.h \
	$$PWD/../include/utils/abstractTimer.h \
	$$PWD/../include/utils/realTimeline.h \
	$$PWD/../include/utils/realTimer.h \
	$$PWD/../include/utils/tracer.h \
	$$PWD/../include/utils/robotCommunication/robotCommunicationThreadInterface.h \
	$$PWD/../include/utils/robotCommunication/robotCommunicator.h \
	$$PWD/../include/utils/robotCommunication/robotCommunicationException.h \
	$$PWD/../include/utils/requiredVersion.h \
	$$PWD/../include/utils/tcpRobotCommunicator.h \
	$$PWD/../include/utils/tcpConnectionHandler.h \

SOURCES += \
	$$PWD/abstractTimer.cpp \
	$$PWD/realTimeline.cpp \
	$$PWD/realTimer.cpp \
	$$PWD/tracer.cpp \
	$$PWD/robotCommunication/robotCommunicator.cpp \
	$$PWD/robotCommunication/robotCommunicationException.cpp \
	$$PWD/tcpRobotCommunicator.cpp \
	$$PWD/tcpConnectionHandler.cpp \

TRANSLATIONS += $$PWD/../../../../qrtranslations/ru/plugins/robots/robots_utils_ru.ts
