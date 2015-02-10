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
	TrikSpeaker(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);

	virtual void play(const QString &filePath) = 0;
};

}
}
}
