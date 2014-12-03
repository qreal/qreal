#pragma once

#include <QtCore/QByteArray>

#include <interpreterBase/robotModel/robotParts/abstractSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "src/robotModel/real/ev3DirectCommand.h"
#include "commandConstants.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Ev3InputDevice : public QObject
{
	Q_OBJECT

public:
	Ev3InputDevice(utils::robotCommunication::RobotCommunicator &robotCommunicator
			, interpreterBase::robotModel::PortInfo const &port);

	void send(QByteArray command, unsigned const responseSize, QByteArray reading);

	/// Returns a value of port that can be used as corresponding byte in request packages.
	char lowLevelPort() const;

	QByteArray readySiCommand(int port, int sensorMode);
	QByteArray readyRawCommand(int port, int sensorMode);
	QByteArray readyPercentCommand(int port, int sensorMode);

private:
	//enums::inputPort::InputPortEnum parsePort();

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
	char mLowLevelPort;
};

}
}
}
}
