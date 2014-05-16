#pragma once

#include "interpreterBase/robotModel/commonRobotModel.h"

namespace interpreterCore {

/// @todo: Replace it with nullKitInterpreter.
class DefaultRobotModel : public interpreterBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
};

}
