#pragma once

#include <kitBase/robotModel/commonRobotModel.h>

namespace nullKitInterpreter {

/// Robot model for robots whose robotId doesn't found among existing robot models in
/// Model after deserialization
class NullRobotModel : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	explicit NullRobotModel(const QString &kitId);

	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
};

}
