# Copyright 2013-2016 QReal Research Group
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

links(qslog qrkernel qrutils qextserialport)

DEFINES += ROBOTS_UTILS_LIBRARY

HEADERS += \
	$$PWD/include/utils/timelineInterface.h \
	$$PWD/include/utils/abstractTimer.h \
	$$PWD/include/utils/realTimeline.h \
	$$PWD/include/utils/realTimer.h \
	$$PWD/include/utils/objectsSet.h \
	$$PWD/include/utils/circularQueue.h \
	$$PWD/include/utils/robotCommunication/networkCommunicationErrorReporter.h \
	$$PWD/include/utils/robotCommunication/robotCommunicationThreadInterface.h \
	$$PWD/include/utils/robotCommunication/robotCommunicator.h \
	$$PWD/include/utils/robotCommunication/runProgramProtocol.h \
	$$PWD/include/utils/robotCommunication/robotCommunicationException.h \
	$$PWD/include/utils/robotCommunication/stopRobotProtocol.h \
	$$PWD/include/utils/robotCommunication/tcpRobotCommunicator.h \
	$$PWD/include/utils/robotCommunication/uploadProgramProtocol.h \
	$$PWD/include/utils/requiredVersion.h \
	$$PWD/include/utils/uploaderTool.h \
	$$PWD/include/utils/canvas/canvas.h \
	$$PWD/include/utils/canvas/canvasObject.h \
	$$PWD/include/utils/canvas/pointObject.h \
	$$PWD/include/utils/canvas/lineObject.h \
	$$PWD/include/utils/canvas/rectangleObject.h \
	$$PWD/include/utils/canvas/ellipseObject.h \
	$$PWD/include/utils/canvas/arcObject.h \
	$$PWD/include/utils/canvas/textObject.h \
	$$PWD/include/utils/widgets/comPortPicker.h \

HEADERS += \
	$$PWD/src/robotCommunication/protocol.h \
	$$PWD/src/robotCommunication/tcpConnectionHandler.h \
	$$PWD/src/robotCommunication/tcpRobotCommunicatorWorker.h \

SOURCES += \
	$$PWD/src/abstractTimer.cpp \
	$$PWD/src/realTimeline.cpp \
	$$PWD/src/realTimer.cpp \
	$$PWD/src/objectsSet.cpp \
	$$PWD/src/uploaderTool.cpp \
	$$PWD/src/canvas/canvas.cpp \
	$$PWD/src/canvas/canvasObject.cpp \
	$$PWD/src/canvas/pointObject.cpp \
	$$PWD/src/canvas/lineObject.cpp \
	$$PWD/src/canvas/rectangleObject.cpp \
	$$PWD/src/canvas/ellipseObject.cpp \
	$$PWD/src/canvas/arcObject.cpp \
	$$PWD/src/canvas/textObject.cpp \
	$$PWD/src/widgets/comPortPicker.cpp \
	$$PWD/src/robotCommunication/networkCommunicationErrorReporter.cpp \
	$$PWD/src/robotCommunication/protocol.cpp \
	$$PWD/src/robotCommunication/robotCommunicator.cpp \
	$$PWD/src/robotCommunication/robotCommunicationException.cpp \
	$$PWD/src/robotCommunication/runProgramProtocol.cpp \
	$$PWD/src/robotCommunication/stopRobotProtocol.cpp \
	$$PWD/src/robotCommunication/tcpRobotCommunicator.cpp \
	$$PWD/src/robotCommunication/tcpConnectionHandler.cpp \
	$$PWD/src/robotCommunication/tcpRobotCommunicatorWorker.cpp \
	$$PWD/src/robotCommunication/uploadProgramProtocol.cpp \

TRANSLATIONS += \
	$$PWD/../../../qrtranslations/ru/plugins/robots/robots_utils_ru.ts \
	$$PWD/../../../qrtranslations/fr/plugins/robots/robots_utils_fr.ts \
