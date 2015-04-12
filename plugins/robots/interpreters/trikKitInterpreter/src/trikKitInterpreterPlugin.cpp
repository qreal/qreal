#include "trikKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>

#include <twoDModel/engine/twoDModelEngineFacade.h>
#include <qrkernel/settingsManager.h>

using namespace trik;
using namespace qReal;

const Id robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
const Id subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

TrikKitInterpreterPlugin::TrikKitInterpreterPlugin()
	: mRealRobotModel(kitId(), "trikKitRobot") // todo: somewhere generate robotId for each robot
	, mTwoDRobotModel(mRealRobotModel)
	, mBlocksFactory(new blocks::TrikBlocksFactory)
{
	mTwoDRobotModel.setWheelPorts("M3", "M4");
	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModel);

	mTwoDRobotModel.setEngine(modelEngine->engine());
	mTwoDModel.reset(modelEngine);

	mAdditionalPreferences = new TrikAdditionalPreferences({ mRealRobotModel.name() });
}

TrikKitInterpreterPlugin::~TrikKitInterpreterPlugin()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}

	if (mOwnsBlocksFactory) {
		delete mBlocksFactory;
	}
}

void TrikKitInterpreterPlugin::init(const kitBase::KitPluginConfigurator &configurator)
{
	connect(&configurator.eventsForKitPlugin()
			, &kitBase::EventsForKitPluginInterface::robotModelChanged
			, [this](const QString &modelName) { mCurrentlySelectedModelName = modelName; });

	connect(&configurator.qRealConfigurator().systemEvents(), &qReal::SystemEvents::activeTabChanged
			, this, &TrikKitInterpreterPlugin::onActiveTabChanged);

	qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			= configurator.qRealConfigurator().mainWindowInterpretersInterface();

	mTwoDModel->init(configurator.eventsForKitPlugin()
			, configurator.qRealConfigurator().systemEvents()
			, configurator.qRealConfigurator().logicalModelApi()
			, interpretersInterface
			, configurator.interpreterControl());

	mRealRobotModel.setErrorReporter(interpretersInterface.errorReporter());

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mRealRobotModel, &robotModel::real::RealRobotModel::rereadSettings);
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mTwoDRobotModel, &robotModel::twoD::TwoDRobotModel::rereadSettings);
}

QString TrikKitInterpreterPlugin::kitId() const
{
	return "trikKit";
}

QString TrikKitInterpreterPlugin::friendlyKitName() const
{
	return tr("TRIK");
}

QList<kitBase::robotModel::RobotModelInterface *> TrikKitInterpreterPlugin::robotModels()
{
	return {&mRealRobotModel, &mTwoDRobotModel};
}

kitBase::blocksBase::BlocksFactoryInterface *TrikKitInterpreterPlugin::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model);
	mOwnsBlocksFactory = false;
	return mBlocksFactory;
}

kitBase::robotModel::RobotModelInterface *TrikKitInterpreterPlugin::defaultRobotModel()
{
	return &mTwoDRobotModel;
}

QList<kitBase::AdditionalPreferences *> TrikKitInterpreterPlugin::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return {mAdditionalPreferences};
}

QWidget *TrikKitInterpreterPlugin::quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model)
{
	return model.name().toLower().contains("twod")
			? nullptr
			: produceIpAddressConfigurer();
}

QList<qReal::ActionInfo> TrikKitInterpreterPlugin::customActions()
{
	return { mTwoDModel->showTwoDModelWidgetActionInfo() };
}

QList<HotKeyActionInfo> TrikKitInterpreterPlugin::hotKeyActions()
{
	mTwoDModel->showTwoDModelWidgetActionInfo().action()->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));

	HotKeyActionInfo d2ModelActionInfo("Interpreter.Show2dModelForTrik", tr("Show 2d model for TRIK")
			, mTwoDModel->showTwoDModelWidgetActionInfo().action());

	return { d2ModelActionInfo };
}

QString TrikKitInterpreterPlugin::defaultSettingsFile() const
{
	return ":/trikDefaultSettings.ini";
}

QIcon TrikKitInterpreterPlugin::iconForFastSelector(
		const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	return &robotModel == &mRealRobotModel
			? QIcon(":/icons/switch-real-trik.svg")
			: &robotModel == &mTwoDRobotModel
					? QIcon(":/icons/switch-2d.svg")
					: QIcon();
}

kitBase::DevicesConfigurationProvider *TrikKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModel->devicesConfigurationProvider();
}

void TrikKitInterpreterPlugin::onActiveTabChanged(const TabInfo &info)
{
	const Id type = info.rootDiagramId().type();
	const bool enabled = type == robotDiagramType || type == subprogramDiagramType;
	const bool twoDModelEnabled = enabled && mCurrentlySelectedModelName == mTwoDRobotModel.name();
	mTwoDModel->showTwoDModelWidgetActionInfo().action()->setVisible(twoDModelEnabled);
}

QWidget *TrikKitInterpreterPlugin::produceIpAddressConfigurer()
{
	QLineEdit * const quickPreferences = new QLineEdit;
	quickPreferences->setPlaceholderText(tr("Enter robot`s IP-address here..."));
	auto updateQuickPreferences = [quickPreferences]() {
		quickPreferences->setText(qReal::SettingsManager::value("TrikTcpServer").toString());
	};
	updateQuickPreferences();
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged, updateQuickPreferences);
	connect(quickPreferences, &QLineEdit::textChanged, [](const QString &text) {
		qReal::SettingsManager::setValue("TrikTcpServer", text);
	});
	return quickPreferences;
}
