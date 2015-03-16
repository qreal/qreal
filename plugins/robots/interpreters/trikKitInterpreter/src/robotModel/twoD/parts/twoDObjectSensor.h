#pragma once

#include <trikKit/robotModel/parts/trikObjectSensor.h>

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

/// Stub for TRIK color sensor for interpreter.
/// Does nothing until object sensor emulation will be requested for 2D model.
class ObjectSensor : public robotModel::parts::TrikObjectSensor
{
	Q_OBJECT

public:
	ObjectSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	void init() override;
	void detect() override;
	void read() override;
};

}
}
}
}
