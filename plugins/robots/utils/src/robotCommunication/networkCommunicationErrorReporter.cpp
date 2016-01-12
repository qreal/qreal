/* Copyright 2016 Yurii Litvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "utils/robotCommunication/networkCommunicationErrorReporter.h"

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "utils/robotCommunication/tcpRobotCommunicator.h"

using namespace utils::robotCommunication;

void NetworkCommunicationErrorReporter::connectErrorReporter(const TcpRobotCommunicator &communicator
		, qReal::ErrorReporterInterface &errorReporter)
{
	QObject::connect(&communicator, &TcpRobotCommunicator::trikRuntimeVersionGettingError, [&errorReporter]() {
		errorReporter.addError(QObject::tr("Current TRIK runtime version can not be received"));
	});

	QObject::connect(&communicator, &TcpRobotCommunicator::trikRuntimeVersionError, [&errorReporter]() {
		errorReporter.addError(QObject::tr("TRIK runtime version is too old, please update it by pressing "
				"'Upload Runtime' button on toolbar"));
	});

	QObject::connect(&communicator, &TcpRobotCommunicator::infoFromRobot, [&errorReporter](const QString &message) {
		errorReporter.addInformation(QObject::tr("From robot: ") + message);
	});

	QObject::connect(&communicator, &TcpRobotCommunicator::errorFromRobot, [&errorReporter](const QString &message) {
		errorReporter.addError(QObject::tr("From robot: ") + message);
	});

	QObject::connect(&communicator, &TcpRobotCommunicator::connectionError, [&errorReporter](const QString &message) {
		errorReporter.addError(message);
	});
}
