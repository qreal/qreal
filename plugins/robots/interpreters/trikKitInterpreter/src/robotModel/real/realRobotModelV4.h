#pragma once

#include "realRobotModelBase.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {

class RealRobotModelV4 : public RealRobotModelBase
{
	Q_OBJECT

public:
	RealRobotModelV4();

	QString name() const override;
	QString friendlyName() const override;
};

}
}
}
