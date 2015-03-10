#pragma once

#include <kitBase/robotModel/robotParts/colorSensor.h>

#include "src/robotModel/real/parts/nxtInputDevice.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorImpl : public QObject
{
	Q_OBJECT

public:
	ColorSensorImpl(const kitBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator
			, enums::lowLevelSensorType::SensorTypeEnum lowLevelType);

	void read();
	void doConfiguration();

signals:
	void newData(int reading);
	void configurationCompleted(bool success);
	void failure();

private slots:
	void sensorSpecificProcessResponse(const QByteArray &reading);

private:
	NxtInputDevice mImplementation;
	enums::lowLevelSensorType::SensorTypeEnum mLowLevelType;
};

}
}
}
}
