#pragma once

#include <interpreterBase/kitPluginInterface.h>
#include "robotModel/real/realRobotModel.h"
#include "robotModel/twoD/twoDRobotModel.h"

namespace nxtKitInterpreter {

class NxtKitInterpreterPlugin : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nxtKitInterpreter.NxtKitInterpreterPlugin")

public:
	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	interpreterBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	// Transfers ownership.
	QWidget *settingsWidget() const override;

//	qReal::IdList specificBlocks() const override;

	// Override.
	qReal::IdList unsupportedBlocks() const override;

private:
	robotModel::real::RealRobotModel mRealRobotModel;
	robotModel::real::TwoDRobotModel mTwoDRobotModel;
};

}
