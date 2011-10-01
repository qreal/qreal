#pragma once
#include "abstractSensorImplementation.h"
#include "../../robotCommandConstants.h"
#include "../../d2RobotModel/d2RobotModel.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class UnrealSensorImplementation : public AbstractSensorImplementation
{
	Q_OBJECT
public:
	UnrealSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model, sensorType::SensorTypeEnum const &sensorType);
	virtual ~UnrealSensorImplementation() {}
	virtual void read() = 0;

protected:
	d2Model::D2RobotModel *mD2Model;
	lowLevelSensorType::SensorTypeEnum mSensorType;
	sensorMode::SensorModeEnum mSensorMode;
	bool mIsConfigured;
	bool mResetDone;
};

}
}
}
}
}
}
