#include "trikKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

#include <commonTwoDModel/engine/twoDModelEngineFacade.h>

#include "src/trikTwoDModelConfigurer.h"

using namespace trikKitInterpreter;
using namespace qReal;

Id const robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
Id const subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

TrikKitInterpreterPlugin::TrikKitInterpreterPlugin()
	: mTwoDRobotModelV4(mRealRobotModelV4)
	, mTwoDRobotModelV6(mRealRobotModelV6)
	, mBlocksFactory(new blocks::TrikBlocksFactory)
{
	mAppTranslator.load(":/trikKitInterpreter_" + QLocale().name());
	QApplication::installTranslator(&mAppTranslator);

	mAdditionalPreferences = new TrikAdditionalPreferences({ mRealRobotModelV4.name(), mRealRobotModelV6.name() });

	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModelV4
			, new TrikTwoDModelConfigurer("M1", "JM3"));

	mTwoDRobotModelV4.setEngine(modelEngine->engine());
	mTwoDModelV4.reset(modelEngine);

	modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModelV6
			, new TrikTwoDModelConfigurer("JM3", "JM4"));

	mTwoDRobotModelV6.setEngine(modelEngine->engine());
	mTwoDModelV6.reset(modelEngine);

	mTwoDModelV4->devicesConfigurationProvider().connectDevicesConfigurationProvider(
			&mTwoDModelV6->devicesConfigurationProvider());

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mRealRobotModelV4, &robotModel::real::RealRobotModelV4::rereadSettings);
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mRealRobotModelV6, &robotModel::real::RealRobotModelV6::rereadSettings);
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mTwoDRobotModelV4, &robotModel::twoD::TwoDRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mTwoDRobotModelV6, &robotModel::twoD::TwoDRobotModel::rereadSettings);
}

void TrikKitInterpreterPlugin::init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
		, SystemEventsInterface const &systemEvents
		, interpreterBase::InterpreterControlInterface &interpreterControl)
{
	connect(&eventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::robotModelChanged
			, [this](QString const &modelName) { mCurrentlySelectedModelName = modelName; });

	connect(&systemEvents, &qReal::SystemEventsInterface::activeTabChanged
			, this, &TrikKitInterpreterPlugin::onActiveTabChanged);

	mTwoDModelV4->init(eventsForKitPlugin, systemEvents, interpreterControl);
	mTwoDModelV6->init(eventsForKitPlugin, systemEvents, interpreterControl);
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
	return {/*&mRealRobotModelV4, */&mRealRobotModelV6/*, &mTwoDRobotModelV4*/, &mTwoDRobotModelV6};
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
	return { /* mTwoDModelV4->showTwoDModelWidgetActionInfo(), */mTwoDModelV6->showTwoDModelWidgetActionInfo() };
}

QList<HotKeyActionInfo> TrikKitInterpreterPlugin::hotKeyActions()
{
	mTwoDModelV4->showTwoDModelWidgetActionInfo().action()->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
	mTwoDModelV6->showTwoDModelWidgetActionInfo().action()->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));

	HotKeyActionInfo d2V4ModelActionInfo("Interpreter.Show2dModelForTrikV4", tr("Show 2d model for TRIK v4")
			, mTwoDModelV4->showTwoDModelWidgetActionInfo().action());
	HotKeyActionInfo d2V6ModelActionInfo("Interpreter.Show2dModelForTrikV6", tr("Show 2d model for TRIK v6")
			, mTwoDModelV6->showTwoDModelWidgetActionInfo().action());

	return { d2V4ModelActionInfo, d2V6ModelActionInfo };
}

QIcon TrikKitInterpreterPlugin::iconForFastSelector(
		interpreterBase::robotModel::RobotModelInterface const &robotModel) const
{
	/// @todo: draw icons for v4 and v6
	return &robotModel == &mRealRobotModelV4 || &robotModel == &mRealRobotModelV6
			? QIcon(":/icons/switch-real-trik.svg")
			: &robotModel == &mTwoDRobotModelV4 || &robotModel == &mTwoDRobotModelV6
					? QIcon(":/icons/switch-2d.svg")
					: QIcon();
}

interpreterBase::DevicesConfigurationProvider *TrikKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModelV4->devicesConfigurationProvider();
}

void TrikKitInterpreterPlugin::onActiveTabChanged(Id const &rootElementId)
{
	bool enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;
	bool v4Enabled = enabled && mCurrentlySelectedModelName == mTwoDRobotModelV4.name();
	bool v6Enabled = enabled && mCurrentlySelectedModelName == mTwoDRobotModelV6.name();
	mTwoDModelV4->showTwoDModelWidgetActionInfo().action()->setVisible(v4Enabled);
	mTwoDModelV6->showTwoDModelWidgetActionInfo().action()->setVisible(v6Enabled);
}
