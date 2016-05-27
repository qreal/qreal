/* Copyright 2016 Grigorii Zimin
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

#include "trikKitInterpreterCommon/robotModel/real/parts/powerMotorsAggregator.h"

#include <QtCore/QPair>

#include <qrutils/inFile.h>

using namespace trik::robotModel::real::parts;

PowerMotorsAggregator::PowerMotorsAggregator(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikMotorsAggregator(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void PowerMotorsAggregator::on(const QList<QPair<QString, int>> &powerForMotors)
{
	const QString pathToCommand = ":/trikQts/templates/engines/forward.t";
	const QString command = utils::InFile::readAll(pathToCommand);

	QString directCommand;

	for (const QPair<QString, int> &motorAndPower : powerForMotors) {
		directCommand += command;
		directCommand
				.replace("@@PORT@@", "\"" + motorAndPower.first + "\"")
				.replace("@@POWER@@", QString::number(motorAndPower.second));
	}

	mRobotCommunicator.runDirectCommand(directCommand);
}

void PowerMotorsAggregator::stop(const QStringList &ports)
{
	QList<QPair<QString, int>> pairList;
	for (const QString &str : ports) {
		pairList.append(qMakePair<QString, int>(str, 0));
	}

	on(pairList);
}

void PowerMotorsAggregator::off(const QStringList &ports)
{
	stop(ports);
}
