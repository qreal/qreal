#pragma once

#include <QtCore/QScopedPointer>

#include <interpreterBase/kitPluginInterface.h>
#include <commonTwoDModel/engine/twoDModelControlInterface.h>

#include "nxtAdditionalPreferences.h"
#include "blocks/nxtBlocksFactory.h"
#include "robotModel/real/realRobotModel.h"
#include "robotModel/twoD/twoDRobotModel.h"

namespace nxtKitInterpreter {

class NxtKitInterpreterPlugin : public QObject, public interpreterBase::KitPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(interpreterBase::KitPluginInterface)
	Q_PLUGIN_METADATA(IID "nxtKitInterpreter.NxtKitInterpreterPlugin")

public:
	NxtKitInterpreterPlugin();

	void init(interpreterBase::EventsForKitPluginInterface &eventsForKitPlugin
			, interpreterBase::InterpreterControlInterface &interpreterControl) override;

	QString kitId() const override;

	QString friendlyKitName() const override;

	QList<interpreterBase::robotModel::RobotModelInterface *> robotModels() override;

	interpreterBase::blocksBase::BlocksFactoryInterface *blocksFactoryFor(
			interpreterBase::robotModel::RobotModelInterface const *model) override;

	interpreterBase::robotModel::RobotModelInterface *defaultRobotModel() override;

	// Transfers ownership.
	interpreterBase::AdditionalPreferences *settingsWidget() override;

//	qReal::IdList specificBlocks() const override;

	qReal::IdList unsupportedBlocks() const override;

	QList<qReal::ActionInfo> customActions() override;

	interpreterBase::SensorsConfigurationProvider * sensorsConfigurationProvider() override;

private:
	robotModel::real::RealRobotModel mRealRobotModel;
	robotModel::twoD::TwoDRobotModel mTwoDRobotModel;
	blocks::NxtBlocksFactory mBlocksFactory;
	NxtAdditionalPreferences *mAdditionalPreferences;  // Transfers ownership
	QScopedPointer<twoDModel::TwoDModelControlInterface> mTwoDModel;
	interpreterBase::InterpreterControlInterface *mInterpreterControl;  // Does not have ownership.
};

}
