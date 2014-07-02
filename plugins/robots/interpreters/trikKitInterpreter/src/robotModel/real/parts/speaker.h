#pragma once

#include "robotModel/parts/trikSpeaker.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Speaker : public robotModel::parts::TrikSpeaker
{
	Q_OBJECT

public:
	Speaker(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void play(QString const &filePath) override;
};

}
}
}
}
