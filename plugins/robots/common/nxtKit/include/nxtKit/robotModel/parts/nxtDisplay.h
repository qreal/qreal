#pragma once

#include <kitBase/robotModel/robotParts/display.h>

namespace nxt {
namespace robotModel {
namespace parts {

class NxtDisplay : public kitBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	NxtDisplay(const kitBase::robotModel::DeviceInfo &info, const kitBase::robotModel::PortInfo &port);

	virtual void drawCircle(int x, int y, int radius) = 0;
};

}
}
}
