#pragma once

#include <interpreterBase/robotModel/commonRobotModel.h>

namespace ev3KitInterpreter {

class Ev3RobotModel : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	explicit Ev3RobotModel(QString const &kitId);

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
};

}
