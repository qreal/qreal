#pragma once

#include "robotModel/parts/trikCameraLineDetector.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class CameraLineDetector : public robotModel::parts::TrikCameraLineDetector
{
	Q_OBJECT

public:
	CameraLineDetector(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void init() override;
	void detectLine() override;
	void read() override;
};

}
}
}
}
