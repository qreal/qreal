#include "robotsPluginFacade.h"

#include "src/coreBlocks/coreBlocksFactory.h"

using namespace interpreterCore;

RobotsPluginFacade::RobotsPluginFacade()
	: mParser(nullptr)
	, mInterpreter(nullptr)
	, mKitPluginManager("plugins/kitPlugins")
	, mActionsManager(/*mKitPluginManager*/)
	, mDockSensorsConfigurer(nullptr)
	, mGraphicsWatcherManager(nullptr)
{
	mRobotSettingsPage = new ui::RobotsSettingsPage(mKitPluginManager, mRobotModelManager);
}

RobotsPluginFacade::~RobotsPluginFacade()
{
	delete mInterpreter;
	delete mParser;
}

void RobotsPluginFacade::init(qReal::PluginConfigurator const &configurer)
{
	mSensorsConfigurationManager.reset(new SensorsConfigurationManager(
			configurer.graphicalModelApi()
			, configurer.logicalModelApi()
			, configurer.mainWindowInterpretersInterface()
			, configurer.systemEvents()
			));

	mTitlesVisibilityManager.reset(
			new TitlesVisibilityManager(mActionsManager.titlesVisibilityAction(), configurer.sceneCustomizer())
			);

	if (!selectKit(configurer)) {
		/// @todo Correctly handle unselected kit.
		return;
	}

	mParser = new textLanguage::RobotsBlockParser(configurer.mainWindowInterpretersInterface().errorReporter());

	initSensorWidgets();
	/// @todo connect 2d model
	/// @todo connect configuration serialization/deserialization into repository

	/// @todo Load currently selected model from registry.
	/// @todo Pass nullptr here in case when there is no kit.
//	mRobotModelManager.setModel(mKitPluginManager.selectedKit().defaultRobotModel());

	interpreterBase::blocksBase::BlocksFactoryInterface * const blocksFactory =
			new coreBlocks::CoreBlocksFactory(
			configurer.graphicalModelApi()
			, configurer.logicalModelApi()
			, mRobotModelManager
			, *configurer.mainWindowInterpretersInterface().errorReporter()
			);

	mBlocksFactoryManager.addFactory(blocksFactory);

	interpreter::Interpreter *interpreter = new interpreter::Interpreter(
			configurer.graphicalModelApi()
			, configurer.logicalModelApi()
			, configurer.mainWindowInterpretersInterface()
			, configurer.projectManager()
			, mBlocksFactoryManager
			, mRobotModelManager
			, *mParser
			, mActionsManager.connectToRobotAction()
			);

	mInterpreter = interpreter;
	mSensorsConfigurationManager->connectSensorsConfigurationProvider(interpreter);

	connectInterpreterToActions();

	connect(&mActionsManager.robotSettingsAction(), &QAction::triggered
			, [=] () { configurer.mainWindowInterpretersInterface().openSettingsDialog(tr("Robots")); });

	blocksFactory->setParser(mParser);
}

PreferencesPage *RobotsPluginFacade::robotsSettingsPage() const
{
	return mRobotSettingsPage;
}

interpreterCore::Customizer &RobotsPluginFacade::customizer()
{
	return mCustomizer;
}

ActionsManager &RobotsPluginFacade::actionsManager()
{
	return mActionsManager;
}

void RobotsPluginFacade::connectInterpreterToActions()
{
	QObject::connect(
			&mActionsManager.runAction()
			, &QAction::triggered
			, mInterpreter
			, &interpreter::InterpreterInterface::interpret
			);

	QObject::connect(
			&mActionsManager.stopRobotAction()
			, &QAction::triggered
			, mInterpreter
			, &interpreter::InterpreterInterface::stopRobot
			);

	QObject::connect(
			&mActionsManager.connectToRobotAction()
			, &QAction::triggered
			, mInterpreter
			, &interpreter::InterpreterInterface::connectToRobot
			);
}

bool RobotsPluginFacade::selectKit(PluginConfigurator const &configurer)
{
	/// @todo reinit it each time when robot model changes
	QString const selectedKit = SettingsManager::value("SelectedRobotKit").toString();
	if (selectedKit.isEmpty() && !mKitPluginManager.kitIds().isEmpty()) {
		SettingsManager::setValue("SelectedRobotKit", mKitPluginManager.kitIds()[0]);
	} else if (mKitPluginManager.kitIds().isEmpty()) {
		configurer.mainWindowInterpretersInterface().setEnabledForAllElementsInPalette(false);

		/// @todo Correctly handle unselected kit.
		return false;
	}

	return true;
}

void RobotsPluginFacade::initSensorWidgets()
{
	mDockSensorsConfigurer = new ui::SensorsConfigurationWidget(nullptr, true);
	mDockSensorsConfigurer->loadRobotModels(mKitPluginManager.allRobotModels());
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, mDockSensorsConfigurer, &ui::SensorsConfigurationWidget::selectRobotModel);
	mDockSensorsConfigurer->selectRobotModel(mRobotModelManager.model());

	mWatchListWindow = new utils::WatchListWindow(mParser);
	mGraphicsWatcherManager = new GraphicsWatcherManager(mParser, this);

	mCustomizer.placeSensorsConfig(mDockSensorsConfigurer);
	mCustomizer.placeWatchPlugins(mWatchListWindow, mGraphicsWatcherManager->widget());

	mSensorsConfigurationManager->connectSensorsConfigurationProvider(mRobotSettingsPage);
	mSensorsConfigurationManager->connectSensorsConfigurationProvider(mDockSensorsConfigurer);
	mSensorsConfigurationManager->connectSensorsConfigurationProvider(mGraphicsWatcherManager);
}
