#pragma once

#include <kitBase/robotModel/robotParts/display.h>

namespace ev3 {
namespace robotModel {
namespace parts {

class Ev3Display : public kitBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	Ev3Display(const kitBase::robotModel::DeviceInfo &info, const kitBase::robotModel::PortInfo &port);

	virtual void drawCircle(int x, int y, int radius, bool filled) = 0;
	virtual void drawRect(int x, int y, int width, int height, bool filled) = 0;
};

}
}
}
