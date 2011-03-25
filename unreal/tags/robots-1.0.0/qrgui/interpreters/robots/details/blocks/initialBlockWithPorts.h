#pragma once

#include <QtCore/QObject>

#include "block.h"
#include "../robotParts/robotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace blocks {

class InitialBlockWithPorts : public Block
{
	Q_OBJECT

public:
	InitialBlockWithPorts(RobotModel &robotModel);
	virtual void run();

	virtual QList<SensorPortPair> usedSensors() const;
	virtual QList<SensorPortPair> sensorToPair(QString const port1Sensor, inputPort::InputPortEnum const &port) const;

private slots:
	void connected(bool success);

private:
	RobotModel &mRobotModel;
};

}
}
}
}
}
