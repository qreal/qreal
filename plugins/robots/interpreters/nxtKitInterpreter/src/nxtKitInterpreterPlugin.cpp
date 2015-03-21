#include "nxtKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>

#include <twoDModel/engine/twoDModelEngineFacade.h>

using namespace nxt;
using namespace qReal;

const Id robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
const Id subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

NxtKitInterpreterPlugin::NxtKitInterpreterPlugin()
	: mUsbRealRobotModel(kitId(), "nxtKitUsbRobot") // todo: somewhere generate robotId for each robot
	, mBluetoothRealRobotModel(kitId(), "nxtKitBluetoothRobot")
	, mTwoDRobotModel(mUsbRealRobotModel)
	, mBlocksFactory(new blocks::NxtBlocksFactory)
{
	mAdditionalPreferences = new NxtAdditionalPreferences(mBluetoothRealRobotModel.name());

	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModel);

	mTwoDRobotModel.setEngine(modelEngine->engine());
	mTwoDModel.reset(modelEngine);

	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mUsbRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &NxtAdditionalPreferences::settingsChanged
			, &mBluetoothRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
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

void NxtKitInterpreterPlugin::init(const kitBase::KitPluginConfigurator &configurator)
{
	connect(&configurator.eventsForKitPlugin(), &kitBase::EventsForKitPluginInterface::robotModelChanged
			, [this](const QString &modelName) { mCurrentlySelectedModelName = modelName; });

	connect(&configurator.qRealConfigurator().systemEvents(), &qReal::SystemEvents::activeTabChanged
			, this, &NxtKitInterpreterPlugin::onActiveTabChanged);

	qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			= configurator.qRealConfigurator().mainWindowInterpretersInterface();
	connect(&mUsbRealRobotModel, &robotModel::real::RealRobotModel::errorOccured
			, [&interpretersInterface](const QString &message) {
				interpretersInterface.errorReporter()->addError(message);
	});
	connect(&mBluetoothRealRobotModel, &robotModel::real::RealRobotModel::errorOccured
			, [&interpretersInterface](const QString &message) {
				interpretersInterface.errorReporter()->addError(message);
	});
	mUsbRealRobotModel.checkConnection();
	mBluetoothRealRobotModel.checkConnection();

	mTwoDModel->init(configurator.eventsForKitPlugin()
			, configurator.qRealConfigurator().systemEvents()
			, configurator.qRealConfigurator().graphicalModelApi()
			, configurator.qRealConfigurator().logicalModelApi()
			, interpretersInterface
			, configurator.interpreterControl());

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
	return {&mUsbRealRobotModel, &mBluetoothRealRobotModel, &mTwoDRobotModel};
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
	return &robotModel == &mUsbRealRobotModel
			? QIcon(":/icons/switch-real-nxt-usb.svg")
			: &robotModel == &mBluetoothRealRobotModel
					? QIcon(":/icons/switch-real-nxt-bluetooth.svg")
					: QIcon(":/icons/switch-2d.svg");
}

kitBase::DevicesConfigurationProvider * NxtKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModel->devicesConfigurationProvider();
}

void NxtKitInterpreterPlugin::onActiveTabChanged(const TabInfo &info)
{
	const Id type = info.rootDiagramId().type();
	const bool enabled = (type == robotDiagramType || type == subprogramDiagramType)
			&& mCurrentlySelectedModelName == mTwoDRobotModel.name();
	mTwoDModel->showTwoDModelWidgetActionInfo().action()->setVisible(enabled);
}
