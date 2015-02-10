#pragma once

#include <interpreterBase/robotModel/robotParts/device.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikLed : public interpreterBase::robotModel::robotParts::Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "led")
	Q_CLASSINFO("friendlyName", tr("Led"))
	Q_CLASSINFO("direction", "output")

public:
	TrikLed(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);

	/// @todo: led color enum here
	virtual void setColor(const QString &color) = 0;
};

}
}
}
