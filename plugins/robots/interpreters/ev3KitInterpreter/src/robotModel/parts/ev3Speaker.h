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
	Ev3Speaker(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void beep(int volume, int duration);
	virtual void playTone(int volume, int frequency, int duration) = 0;
};

}
}
}
