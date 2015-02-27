#include "nxtKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

#include <twoDModel/engine/twoDModelEngineFacade.h>

using namespace nxt;
using namespace qReal;

const Id robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
const Id subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

NxtKitInterpreterPlugin::NxtKitInterpreterPlugin()
	: mRealRobotModel(kitId(), "nxtKitRobot") // todo: somewhere generate robotId for each robot
	, mTwoDRobotModel(mRealRobotModel)
	, mBlocksFactory(new blocks::NxtBlocksFactory)
{
	mAdditionalPreferences = new NxtAdditionalPreferences(mRealRobotModel.name());

	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModel);

	mTwoDRobotModel.setEngine(modelEngine->engine());
	mTwoDModel.reset(modelEngine);

	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mTwoDRobotModel, &robotModel::twoD::TwoDRobotModel::rereadSettings);
}

NxtKitInterpreterPlugin::~NxtKitInterpreterPlugin()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}

	if (mOwnsBlocksFactory) {
		delete mBlocksFactory;
	}
}

void NxtKitInterpreterPlugin::init(const kitBase::EventsForKitPluginInterface &eventsForKitPlugin
		, const SystemEvents &systemEvents
		, qReal::GraphicalModelAssistInterface &graphicalModel
		, qReal::LogicalModelAssistInterface &logicalModel
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, kitBase::InterpreterControlInterface &interpreterControl)
{
	connect(&eventsForKitPlugin, &kitBase::EventsForKitPluginInterface::robotModelChanged
			, [this](const QString &modelName) { mCurrentlySelectedModelName = modelName; });

	connect(&systemEvents, &qReal::SystemEvents::activeTabChanged
			, this, &NxtKitInterpreterPlugin::onActiveTabChanged);

	connect(&mRealRobotModel, &robotModel::real::RealRobotModel::errorOccured
			, [&interpretersInterface](const QString &message) {
				interpretersInterface.errorReporter()->addError(message);
	});
	mRealRobotModel.checkConnection();

	mTwoDModel->init(eventsForKitPlugin, systemEvents, graphicalModel
			, logicalModel, interpretersInterface, interpreterControl);

}

QString NxtKitInterpreterPlugin::kitId() const
{
	return "nxtKit";
}

QString NxtKitInterpreterPlugin::friendlyKitName() const
{
	return tr("Lego NXT");
}

QList<kitBase::robotModel::RobotModelInterface *> NxtKitInterpreterPlugin::robotModels()
{
	return {&mRealRobotModel, &mTwoDRobotModel};
}

kitBase::blocksBase::BlocksFactoryInterface *NxtKitInterpreterPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model);
	mOwnsBlocksFactory = false;
	return mBlocksFactory;
}

kitBase::robotModel::RobotModelInterface *NxtKitInterpreterPlugin::defaultRobotModel()
{
	return &mTwoDRobotModel;
}

QList<kitBase::AdditionalPreferences *> NxtKitInterpreterPlugin::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return {mAdditionalPreferences};
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
		const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	return &robotModel == &mRealRobotModel
			? QIcon(":/icons/switch-real-nxt.svg")
			: QIcon(":/icons/switch-2d.svg");
}

kitBase::DevicesConfigurationProvider * NxtKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModel->devicesConfigurationProvider();
}

void NxtKitInterpreterPlugin::onActiveTabChanged(const Id &rootElementId)
{
	bool enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;
	enabled &= mCurrentlySelectedModelName == mTwoDRobotModel.name();
	mTwoDModel->showTwoDModelWidgetActionInfo().action()->setVisible(enabled);
}
