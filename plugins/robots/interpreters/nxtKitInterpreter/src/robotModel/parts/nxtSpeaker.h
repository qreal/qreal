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
	NxtSpeaker(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void beep(unsigned time);
	virtual void playTone(unsigned freq, unsigned time) = 0;

private:
	void doConfiguration() override;
};

}
}
}
