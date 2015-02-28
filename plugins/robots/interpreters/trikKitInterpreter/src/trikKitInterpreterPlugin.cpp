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
	: mRealRobotModelV6(kitId(), "trikKitRobot") // todo: somewhere generate robotId for each robot
	, mTwoDRobotModelV6(mRealRobotModelV6)
	, mBlocksFactory(new blocks::TrikBlocksFactory)
{
	mTwoDRobotModelV6.setWheelPorts("M3", "M4");
	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModelV6);

	mTwoDRobotModelV6.setEngine(modelEngine->engine());
	mTwoDModelV6.reset(modelEngine);

	mAdditionalPreferences = new TrikAdditionalPreferences({ mRealRobotModelV6.name() });
	mFSharpAdditionalPreferences = new TrikFSharpAdditionalPreferences();
}

TrikKitInterpreterPlugin::~TrikKitInterpreterPlugin()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
		delete mFSharpAdditionalPreferences;
	}

	if (mOwnsBlocksFactory) {
		delete mBlocksFactory;
	}

	if (mOwnsIpAdressQuickConfigurer) {
		delete mIpAdressQuickConfigurer;
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

	mTwoDModelV6->init(configurator.eventsForKitPlugin()
			, configurator.qRealConfigurator().systemEvents()
			, configurator.qRealConfigurator().graphicalModelApi()
			, configurator.qRealConfigurator().logicalModelApi()
			, interpretersInterface
			, configurator.interpreterControl());

	mRealRobotModelV6.setErrorReporter(interpretersInterface.errorReporter());

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
	mIpAdressQuickConfigurer = quickPreferences;

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mRealRobotModelV6, &robotModel::real::RealRobotModelV6::rereadSettings);
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mTwoDRobotModelV6, &robotModel::twoD::TwoDRobotModel::rereadSettings);
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
	return {&mRealRobotModelV6, &mTwoDRobotModelV6};
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
	return &mTwoDRobotModelV6;
}

QList<kitBase::AdditionalPreferences *> TrikKitInterpreterPlugin::settingsWidgets()
{
	mOwnsAdditionalPreferences = false;
	return {mAdditionalPreferences, mFSharpAdditionalPreferences};
}

QWidget *TrikKitInterpreterPlugin::quickPreferencesFor(const kitBase::robotModel::RobotModelInterface &model)
{
	if (model.name().toLower().contains("twod")) {
		return nullptr;
	} else {
		mOwnsIpAdressQuickConfigurer = false;
		return mIpAdressQuickConfigurer;
	}
}

QList<qReal::ActionInfo> TrikKitInterpreterPlugin::customActions()
{
	return { mTwoDModelV6->showTwoDModelWidgetActionInfo() };
}

QList<HotKeyActionInfo> TrikKitInterpreterPlugin::hotKeyActions()
{
	mTwoDModelV6->showTwoDModelWidgetActionInfo().action()->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));

	HotKeyActionInfo d2V6ModelActionInfo("Interpreter.Show2dModelForTrikV6", tr("Show 2d model for TRIK v6")
			, mTwoDModelV6->showTwoDModelWidgetActionInfo().action());

	return { d2V6ModelActionInfo };
}

QString TrikKitInterpreterPlugin::defaultSettingsFile() const
{
	return ":/trikDefaultSettings.ini";
}

QIcon TrikKitInterpreterPlugin::iconForFastSelector(
		const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	/// @todo: draw icons for v6
	return &robotModel == &mRealRobotModelV6
			? QIcon(":/icons/switch-real-trik.svg")
			: &robotModel == &mTwoDRobotModelV6
					? QIcon(":/icons/switch-2d.svg")
					: QIcon();
}

kitBase::DevicesConfigurationProvider *TrikKitInterpreterPlugin::devicesConfigurationProvider()
{
	return &mTwoDModelV6->devicesConfigurationProvider();
}

void TrikKitInterpreterPlugin::onActiveTabChanged(const Id &rootElementId)
{
	const bool enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;
	const bool v6Enabled = enabled && mCurrentlySelectedModelName == mTwoDRobotModelV6.name();
	mTwoDModelV6->showTwoDModelWidgetActionInfo().action()->setVisible(v6Enabled);
}
