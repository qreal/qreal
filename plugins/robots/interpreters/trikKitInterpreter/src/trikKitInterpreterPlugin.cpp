#include "trikKitInterpreterPlugin.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>

#include <commonTwoDModel/engine/twoDModelEngineFacade.h>
#include <qrkernel/settingsManager.h>

using namespace trikKitInterpreter;
using namespace qReal;

Id const robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
Id const subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");

TrikKitInterpreterPlugin::TrikKitInterpreterPlugin()
	: mRealRobotModelV6(kitId())
	, mTwoDRobotModelV6(mRealRobotModelV6)
	, mBlocksFactory(new blocks::TrikBlocksFactory)
{
	mAdditionalPreferences = new TrikAdditionalPreferences({ mRealRobotModelV6.name() });

	QLineEdit * const quickPreferences = new QLineEdit;
	quickPreferences->setPlaceholderText(tr("Enter robot`s IP-address here..."));
	auto updateQuickPreferences = [quickPreferences]() {
		quickPreferences->setText(qReal::SettingsManager::value("TrikTcpServer").toString());
	};
	updateQuickPreferences();
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged, updateQuickPreferences);
	connect(quickPreferences, &QLineEdit::textChanged, [](QString const &text) {
		qReal::SettingsManager::setValue("TrikTcpServer", text);
	});
	mIpAdressQuicksConfigurer = quickPreferences;

	auto modelEngine = new twoDModel::engine::TwoDModelEngineFacade(mTwoDRobotModelV6);

	mTwoDRobotModelV6.setEngine(modelEngine->engine());
	mTwoDModelV6.reset(modelEngine);

	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mRealRobotModelV6, &robotModel::real::RealRobotModelV6::rereadSettings);
	connect(mAdditionalPreferences, &TrikAdditionalPreferences::settingsChanged
			, &mTwoDRobotModelV6, &robotModel::twoD::TwoDRobotModel::rereadSettings);
}

TrikKitInterpreterPlugin::~TrikKitInterpreterPlugin()
{
	if (mOwnsAdditionalPreferences) {
		delete mAdditionalPreferences;
	}

	if (mOwnsBlocksFactory) {
		delete mBlocksFactory;
	}

	if (mOwnsIpAdressQuicksConfigurer) {
		delete mIpAdressQuicksConfigurer;
	}
}

void TrikKitInterpreterPlugin::init(interpreterBase::EventsForKitPluginInterface const &eventsForKitPlugin
		, SystemEventsInterface const &systemEvents
		, qReal::GraphicalModelAssistInterface &graphicalModel
		, qReal::LogicalModelAssistInterface &logicalModel
		, qReal::gui::MainWindowInterpretersInterface const &interpretersInterface
		, interpreterBase::InterpreterControlInterface &interpreterControl)
{
	connect(&eventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::robotModelChanged
			, [this](QString const &modelName) { mCurrentlySelectedModelName = modelName; });

	connect(&systemEvents, &qReal::SystemEventsInterface::activeTabChanged
			, this, &TrikKitInterpreterPlugin::onActiveTabChanged);

	mTwoDModelV6->init(eventsForKitPlugin, systemEvents, graphicalModel
			, logicalModel, interpretersInterface, interpreterControl);
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
	mOwnsBlocksFactory = false;
	return mBlocksFactory;
}

interpreterBase::robotModel::RobotModelInterface *TrikKitInterpreterPlugin::defaultRobotModel()
{
	return &mTwoDRobotModelV6;
}

interpreterBase::AdditionalPreferences *TrikKitInterpreterPlugin::settingsWidget()
{
	mOwnsAdditionalPreferences = false;
	return mAdditionalPreferences;
}

QWidget *TrikKitInterpreterPlugin::quickPreferencesFor(interpreterBase::robotModel::RobotModelInterface const &model)
{
	if (model.name().toLower().contains("twod")) {
		return nullptr;
	} else {
		mOwnsIpAdressQuicksConfigurer = false;
		return mIpAdressQuicksConfigurer;
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
