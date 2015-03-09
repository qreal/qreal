#pragma once

#include "robotModel/parts/ev3Speaker.h"
#include <utils/robotCommunication/robotCommunicator.h>

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Speaker : public robotModel::parts::Ev3Speaker
{
	Q_OBJECT

public:
	Speaker(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	/// @param volume.Volume to play the tone (0-100).
	/// @param frequency.Frequency of the tone in hertz.
	/// @param duration.Duration of the tone in milliseconds.
	void playTone(int volume, int frequency, int duration) override;

private:
	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}
