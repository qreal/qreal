#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace nullKitInterpreter {

class NullRobotModel : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
};

}
