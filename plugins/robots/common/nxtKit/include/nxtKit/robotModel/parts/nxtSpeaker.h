#pragma once

#include <kitBase/robotModel/robotParts/speaker.h>

namespace nxt {
namespace robotModel {
namespace parts {

class NxtSpeaker : public kitBase::robotModel::robotParts::Speaker
{
	Q_OBJECT

public:
	NxtSpeaker(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	void beep(unsigned time);
	virtual void playTone(unsigned freq, unsigned time) = 0;
};

}
}
}
