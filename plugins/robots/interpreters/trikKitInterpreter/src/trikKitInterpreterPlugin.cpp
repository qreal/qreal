#include "trikKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

#include <commonTwoDModel/engine/twoDModelEngineFacade.h>

#include "src/trikTwoDModelConfigurer.h"

using namespace trikKitInterpreter;
using namespace qReal;

Id const robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
Id const subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

TrikKitInterpreterPlugin::TrikKitInterpreterPlugin()
	: mRealRobotModelV6(kitId())
	, mTwoDRobotModelV6(mRealRobotModelV6)
	, mBlocksFactory(new blocks::TrikBlocksFactory)
{
	mAppTranslator.load(":/trikKitInterpreter_" + QLocale().name());
	QApplication::installTranslator(&mAppTranslator);

	mAdditionalPreferences = new TrikAdditionalPreferences({ mRealRobotModelV6.name() });

	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModelV6
			, new TrikTwoDModelConfigurer("M3", "M4"));

	mTwoDRobotModelV6.setEngine(modelEngine->engine());
	mTwoDModelV6.reset(modelEngine);

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mRealRobotModelV6, &robotModel::real::RealRobotModelV6::rereadSettings);
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mTwoDRobotModelV6, &robotModel::twoD::TwoDRobotModel::rereadSettings);
}

void TrikKitInterpreterPlugin::init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
		, SystemEventsInterface const &systemEvents
		, qReal::GraphicalModelAssistInterface &graphicalModel
		, qReal::LogicalModelAssistInterface &logicalModel
		, interpreterBase::InterpreterControlInterface &interpreterControl)
{
	connect(&eventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::robotModelChanged
			, [this](QString const &modelName) { mCurrentlySelectedModelName = modelName; });

	connect(&systemEvents, &qReal::SystemEventsInterface::activeTabChanged
			, this, &TrikKitInterpreterPlugin::onActiveTabChanged);

	mTwoDModelV6->init(eventsForKitPlugin, systemEvents, graphicalModel, logicalModel, interpreterControl);
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
	return {&mRealRobotModelV6, &mTwoDRobotModelV6};
}

interpreterBase::blocksBase::BlocksFactoryInterface *TrikKitInterpreterPlugin::blocksFactoryFor(
		interpreterBase::robotModel::RobotModelInterface const *model)
{
	Q_UNUSED(model);
	return mBlocksFactory;
}

interpreterBase::robotModel::RobotModelInterface *TrikKitInterpreterPlugin::defaultRobotModel()
{
	return &mTwoDRobotModelV6;
}

interpreterBase::AdditionalPreferences *TrikKitInterpreterPlugin::settingsWidget()
{
	return mAdditionalPreferences;
}

QList<qReal::ActionInfo> TrikKitInterpreterPlugin::customActions()
{
	return { mTwoDModelV6->showTwoDModelWidgetActionInfo() };
}

QList<HotKeyActionInfo> TrikKitInterpreterPlugin::hotKeyActions()
{
	mTwoDModelV6->showTwoDModelWidgetActionInfo().action()->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
	mTwoDModelV6->showTwoDModelWidgetActionInfo().action()->setObjectName("show2dModelTrik");

	HotKeyActionInfo d2V6ModelActionInfo("Interpreter.Show2dModelForTrikV6", tr("Show 2d model for TRIK v6")
			, mTwoDModelV6->showTwoDModelWidgetActionInfo().action());

	return { d2V6ModelActionInfo };
}

QIcon TrikKitInterpreterPlugin::iconForFastSelector(
		interpreterBase::robotModel::RobotModelInterface const &robotModel) const
{
	/// @todo: draw icons for v6
	return &robotModel == &mRealRobotModelV6
			? QIcon(":/icons/switch-real-trik.svg")
			: &robotModel == &mTwoDRobotModelV6
					? QIcon(":/icons/switch-2d.svg")
					: QIcon();
}

interpreterBase::DevicesConfigurationProvider *TrikKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModelV6->devicesConfigurationProvider();
}

void TrikKitInterpreterPlugin::onActiveTabChanged(Id const &rootElementId)
{
	bool const enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;
	bool const v6Enabled = enabled && mCurrentlySelectedModelName == mTwoDRobotModelV6.name();
	mTwoDModelV6->showTwoDModelWidgetActionInfo().action()->setVisible(v6Enabled);
}
