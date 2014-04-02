#pragma once

#include <commonTwoDModel/robotModel/twoDRobotModel.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace twoD {

class TwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	TwoDRobotModel();
	QString name() const override;
	QString friendlyName() const override;
};

}
}
}
