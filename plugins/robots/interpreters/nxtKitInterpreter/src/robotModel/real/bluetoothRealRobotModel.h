#pragma once

#include "realRobotModel.h"

namespace nxt {
namespace robotModel {
namespace real {

class BluetoothRealRobotModel : public RealRobotModel
{
	Q_OBJECT

public:
	BluetoothRealRobotModel(const QString &kitId, const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	int priority() const override;
};

}
}
}
