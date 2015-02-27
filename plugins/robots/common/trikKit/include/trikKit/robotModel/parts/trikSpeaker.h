#pragma once

#include <kitBase/robotModel/robotParts/speaker.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace trik {
namespace robotModel {
namespace parts {

class TrikSpeaker : public kitBase::robotModel::robotParts::Speaker
{
	Q_OBJECT

public:
	TrikSpeaker(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	virtual void play(const QString &filePath) = 0;
};

}
}
}
