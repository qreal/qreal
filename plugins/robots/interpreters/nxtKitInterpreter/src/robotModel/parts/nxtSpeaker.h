#pragma once

#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace parts {

class NxtSpeaker : public interpreterBase::robotModel::robotParts::Speaker
{
	Q_OBJECT

public:
	NxtSpeaker(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);

	void beep(unsigned time);
	virtual void playTone(unsigned freq, unsigned time) = 0;
};

}
}
}
