#pragma once

#include <kitBase/robotModel/robotParts/device.h>

namespace trik {
namespace robotModel {
namespace parts {

class TrikLed : public kitBase::robotModel::robotParts::Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "led")
	Q_CLASSINFO("friendlyName", tr("Led"))
	Q_CLASSINFO("direction", "output")

public:
	TrikLed(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// @todo: led color enum here
	virtual void setColor(const QString &color) = 0;
};

}
}
}
