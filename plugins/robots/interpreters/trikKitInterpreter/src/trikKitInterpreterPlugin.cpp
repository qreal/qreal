#include "trikKitInterpreterPlugin.h"

#include <commonTwoDModel/engine/twoDModelEngineFacade.h>

#include "src/trikTwoDModelConfigurer.h"

using namespace trikKitInterpreter;
using namespace qReal;

Id const robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
Id const subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

TrikKitInterpreterPlugin::TrikKitInterpreterPlugin()
	: mBlocksFactory(new blocks::TrikBlocksFactory)
	, mAdditionalPreferences(new TrikAdditionalPreferences(mRealRobotModel.name()))
{
	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModel, new TrikTwoDModelConfigurer());

	mTwoDRobotModel.setEngine(modelEngine->engine());
	mTwoDModel.reset(modelEngine);

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mTwoDRobotModel, &robotModel::twoD::TwoDRobotModel::rereadSettings);
}

void TrikKitInterpreterPlugin::init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
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
			, &TrikKitInterpreterPlugin::onActiveTabChanged);

	mTwoDModel->init();
}

QString TrikKitInterpreterPlugin::kitId() const
{
	return "trikKit";
}

QString TrikKitInterpreterPlugin::friendlyKitName() const
{
	return tr("TRIK");
}

QList<interpreterBase::robotModel::RobotModelInterface *> TrikKitInterpreterPlugin::robotModels()
{
	return {&mRealRobotModel, &mTwoDRobotModel};
}

interpreterBase::blocksBase::BlocksFactoryInterface *TrikKitInterpreterPlugin::blocksFactoryFor(
		interpreterBase::robotModel::RobotModelInterface const *model)
{
	Q_UNUSED(model);
	return mBlocksFactory;
}

interpreterBase::robotModel::RobotModelInterface *TrikKitInterpreterPlugin::defaultRobotModel()
{
	return &mTwoDRobotModel;
}

interpreterBase::AdditionalPreferences *TrikKitInterpreterPlugin::settingsWidget()
{
	return mAdditionalPreferences;
}

QList<qReal::ActionInfo> TrikKitInterpreterPlugin::customActions()
{
	return {mTwoDModel->showTwoDModelWidgetActionInfo()};
}

interpreterBase::DevicesConfigurationProvider *TrikKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModel->devicesConfigurationProvider();
}

void TrikKitInterpreterPlugin::onActiveTabChanged(Id const &rootElementId)
{
	bool const enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;
	mTwoDModel->showTwoDModelWidgetActionInfo().action()->setVisible(enabled);
}
