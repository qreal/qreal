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
	UnrealSensorImplementation(
			robots::enums::inputPort::InputPortEnum const port
			, d2Model::D2RobotModel *d2Model
			, robots::enums::sensorType::SensorTypeEnum const &sensorType
			);

	virtual ~UnrealSensorImplementation() {}
	virtual void read() = 0;
	virtual void configure();

protected:
	d2Model::D2RobotModel *mD2Model;
	enums::lowLevelSensorType::SensorTypeEnum mSensorType;
	enums::sensorMode::SensorModeEnum mSensorMode;
	bool mIsConfigured;
	bool mResetDone;
};

}
}
}
}
}
}
