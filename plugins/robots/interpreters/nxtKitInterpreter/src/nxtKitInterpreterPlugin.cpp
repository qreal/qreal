#include "nxtKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

#include <commonTwoDModel/engine/twoDModelEngineFacade.h>

using namespace nxtKitInterpreter;
using namespace qReal;

Id const robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
Id const subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

NxtKitInterpreterPlugin::NxtKitInterpreterPlugin()
	: mRealRobotModel(kitId())
	, mTwoDRobotModel(mRealRobotModel)
	, mBlocksFactory(new blocks::NxtBlocksFactory)
{
	mAppTranslator.load(":/nxtKitInterpreter_" + QLocale().name());
	QApplication::installTranslator(&mAppTranslator);

	mAdditionalPreferences = new NxtAdditionalPreferences(mRealRobotModel.name());

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
		, qReal::GraphicalModelAssistInterface &graphicalModel
		, qReal::LogicalModelAssistInterface &logicalModel
		, interpreterBase::InterpreterControlInterface &interpreterControl)
{
	connect(&eventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::robotModelChanged
			, [this](QString const &modelName) { mCurrentlySelectedModelName = modelName; });

	connect(&systemEvents
			, &qReal::SystemEventsInterface::activeTabChanged
			, this
			, &NxtKitInterpreterPlugin::onActiveTabChanged);

	mTwoDModel->init(eventsForKitPlugin, systemEvents, graphicalModel, logicalModel, interpreterControl);
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

QList<qReal::ActionInfo> NxtKitInterpreterPlugin::customActions()
{
	return { mTwoDModel->showTwoDModelWidgetActionInfo() };
}

QList<HotKeyActionInfo> NxtKitInterpreterPlugin::hotKeyActions()
{
	mTwoDModel->showTwoDModelWidgetActionInfo().action()->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));

	HotKeyActionInfo d2ModelActionInfo("Interpreter.Show2dModelForNxt", tr("Show 2d model")
			, mTwoDModel->showTwoDModelWidgetActionInfo().action());

	return { d2ModelActionInfo };
}

QString NxtKitInterpreterPlugin::defaultSettingsFile() const
{
	return ":/nxtDefaultSettings.ini";
}

QIcon NxtKitInterpreterPlugin::iconForFastSelector(
		interpreterBase::robotModel::RobotModelInterface const &robotModel) const
{
	return &robotModel == &mRealRobotModel
			? QIcon(":/icons/switch-real-nxt.svg")
			: QIcon(":/icons/switch-2d.svg");
}

interpreterBase::DevicesConfigurationProvider * NxtKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModel->devicesConfigurationProvider();
}

void NxtKitInterpreterPlugin::onActiveTabChanged(Id const &rootElementId)
{
	bool enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;
	enabled &= mCurrentlySelectedModelName == mTwoDRobotModel.name();
	mTwoDModel->showTwoDModelWidgetActionInfo().action()->setVisible(enabled);
}
