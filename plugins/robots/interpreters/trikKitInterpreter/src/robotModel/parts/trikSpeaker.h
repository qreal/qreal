#pragma once

#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikSpeaker : public interpreterBase::robotModel::robotParts::Speaker
{
	Q_OBJECT

public:
	TrikSpeaker(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	virtual void play(QString const &filePath) = 0;
};

}
}
}
