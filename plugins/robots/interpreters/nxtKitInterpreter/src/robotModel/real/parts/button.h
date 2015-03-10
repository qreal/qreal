#pragma once

#include <kitBase/robotModel/robotParts/button.h>

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class Button : public kitBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	Button(const kitBase::robotModel::DeviceInfo &info, const kitBase::robotModel::PortInfo &port);

	void read() override;
};

}
}
}
}
