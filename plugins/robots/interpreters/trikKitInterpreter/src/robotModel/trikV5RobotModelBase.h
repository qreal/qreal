#pragma once

#include "trikV4RobotModelBase.h"

namespace trikKitInterpreter {
namespace robotModel {

class TrikV5RobotModelBase : public TrikV4RobotModelBase
{
	Q_OBJECT

public:
	TrikV5RobotModelBase();

	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;

protected:
	virtual interpreterBase::robotModel::DeviceInfo movementSensorInfo() const;
};

}
}
