#include "nxtKitInterpreterPlugin.h"

#include <commonTwoDModel/engine/twoDModelEngineFacade.h>

using namespace nxtKitInterpreter;

NxtKitInterpreterPlugin::NxtKitInterpreterPlugin()
	: mAdditionalPreferences(new NxtAdditionalPreferences(mRealRobotModel.name()))
{
	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModel);

	mTwoDRobotModel.setEngine(modelEngine->engine());
	mTwoDModel.reset(modelEngine);

	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mTwoDRobotModel, &robotModel::twoD::TwoDRobotModel::rereadSettings);
}

QString NxtKitInterpreterPlugin::kitId() const
{
	return "nxtKit";
}

QString NxtKitInterpreterPlugin::friendlyKitName() const
{
	return tr("Lego NXT");
}

QList<interpreterBase::robotModel::RobotModelInterface *> NxtKitInterpreterPlugin::robotModels()
{
	return {&mRealRobotModel, &mTwoDRobotModel};
}

interpreterBase::blocksBase::BlocksFactoryInterface *NxtKitInterpreterPlugin::blocksFactoryFor(
		interpreterBase::robotModel::RobotModelInterface const *model)
{
	Q_UNUSED(model);
	return &mBlocksFactory;
}

interpreterBase::robotModel::RobotModelInterface *NxtKitInterpreterPlugin::defaultRobotModel()
{
	return &mTwoDRobotModel;
}

interpreterBase::AdditionalPreferences *NxtKitInterpreterPlugin::settingsWidget()
{
	return mAdditionalPreferences;
}

//qReal::IdList NxtKitInterpreterInterface::specificBlocks() const
//{
//	return qReal::IdList();
//}

qReal::IdList NxtKitInterpreterPlugin::unsupportedBlocks() const
{
	return {};
}

QList<qReal::ActionInfo> NxtKitInterpreterPlugin::customActions()
{
	return {mTwoDModel->showTwoDModelWidgetActionInfo()};
}

interpreterBase::SensorsConfigurationProvider * NxtKitInterpreterPlugin::sensorsConfigurationProvider()
{
	return &mTwoDModel->sensorsConfigurationProvider();
}
