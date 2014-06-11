#pragma once

#include "realRobotModelBase.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {

class RealRobotModelV6 : public RealRobotModelBase
{
	Q_OBJECT

public:
	RealRobotModelV6();

	QString name() const override;
	QString friendlyName() const override;
};

}
}
}
