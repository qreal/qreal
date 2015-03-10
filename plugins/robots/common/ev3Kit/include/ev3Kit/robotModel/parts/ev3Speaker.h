#pragma once

#include <kitBase/robotModel/robotParts/speaker.h>

namespace ev3 {
namespace robotModel {
namespace parts {

class Ev3Speaker : public kitBase::robotModel::robotParts::Speaker
{
	Q_OBJECT

public:
	Ev3Speaker(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	void beep(unsigned time);
	virtual void playTone(int volume, int frequency, int duration) = 0;
};

}
}
}
