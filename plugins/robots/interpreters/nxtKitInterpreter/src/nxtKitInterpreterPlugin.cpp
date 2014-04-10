#include "nxtKitInterpreterPlugin.h"

#include <commonTwoDModel/engine/twoDModelEngineFacade.h>

using namespace nxtKitInterpreter;
using namespace qReal;

Id const robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
Id const subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

NxtKitInterpreterPlugin::NxtKitInterpreterPlugin()
	: mBlocksFactory(new blocks::NxtBlocksFactory)
	, mAdditionalPreferences(new NxtAdditionalPreferences(mRealRobotModel.name()))
{
	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModel);

	mTwoDRobotModel.setEngine(modelEngine->engine());
	mTwoDModel.reset(modelEngine);

	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mTwoDRobotModel, &robotModel::twoD::TwoDRobotModel::rereadSettings);
}

void NxtKitInterpreterPlugin::init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
		, SystemEventsInterface const &systemEvents
		, interpreterBase::InterpreterControlInterface &interpreterControl)
{
	connect(&eventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::interpretationStarted
			, mTwoDModel.data()
			, &twoDModel::TwoDModelControlInterface::onStartInterpretation
			);

	connect(&eventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::interpretationStopped
			, mTwoDModel.data()
			, &twoDModel::TwoDModelControlInterface::onStopInterpretation
			);

	connect(mTwoDModel.data()
			, &twoDModel::TwoDModelControlInterface::runButtonPressed
			, &interpreterControl
			, &interpreterBase::InterpreterControlInterface::interpret
			);

	connect(mTwoDModel.data()
			, &twoDModel::TwoDModelControlInterface::stopButtonPressed
			, &interpreterControl
			, &interpreterBase::InterpreterControlInterface::stopRobot
			);

	connect(&systemEvents
			, &qReal::SystemEventsInterface::activeTabChanged
			, this
			, &NxtKitInterpreterPlugin::onActiveTabChanged);

	mTwoDModel->init();
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
	return mBlocksFactory;
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

interpreterBase::DevicesConfigurationProvider * NxtKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModel->devicesConfigurationProvider();
}

void NxtKitInterpreterPlugin::onActiveTabChanged(Id const &rootElementId)
{
	bool const enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;
	mTwoDModel->showTwoDModelWidgetActionInfo().action()->setVisible(enabled);
}
