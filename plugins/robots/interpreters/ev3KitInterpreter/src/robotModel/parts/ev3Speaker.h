#pragma once

#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace ev3KitInterpreter {
namespace robotModel {
namespace parts {

class Ev3Speaker : public interpreterBase::robotModel::robotParts::Speaker
{
	Q_OBJECT

public:
	Ev3Speaker(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);

	void beep(unsigned time);
	virtual void playTone(int volume, int frequency, int duration) = 0;
};

}
}
}
