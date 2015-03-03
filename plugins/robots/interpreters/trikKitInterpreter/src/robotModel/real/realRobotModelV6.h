#pragma once

#include "realRobotModelBase.h"

namespace trik {
namespace robotModel {
namespace real {

class RealRobotModelV6 : public RealRobotModelBase
{
	Q_OBJECT

public:
	RealRobotModelV6(const QString &kitId, const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	int priority() const override;
};

}
}
}
