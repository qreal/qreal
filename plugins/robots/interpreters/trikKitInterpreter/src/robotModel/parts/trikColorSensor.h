#pragma once

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikLineSensor : public interpreterBase::robotModel::robotParts::ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "cameraLineDetector")
	Q_CLASSINFO("friendlyName", tr("Camera Line Detector"))

public:
	TrikLineSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	virtual void init() = 0;
	virtual void detectLine() = 0;
	// read() is obtained from AbstractSensor
};

}
}
}
