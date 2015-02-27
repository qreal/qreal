#pragma once

#include "kitBase/robotModel/commonRobotModel.h"

namespace interpreterCore {

/// Default robot model that is used when no kit plugin with models is loaded.
/// @todo: Replace it with nullKitInterpreter.
class DefaultRobotModel : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	DefaultRobotModel();
	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
};

}
