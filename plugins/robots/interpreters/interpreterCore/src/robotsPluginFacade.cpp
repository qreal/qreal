#include "robotsPluginFacade.h"

#include "src/coreBlocks/coreBlocksFactory.h"
#include "managers/paletteUpdateManager.h"

using namespace interpreterCore;

RobotsPluginFacade::RobotsPluginFacade()
	: mParser(nullptr)
	, mInterpreter(nullptr)
	, mKitPluginManager("plugins/kitPlugins")
	, mActionsManager(mKitPluginManager, mRobotModelManager)
	, mDockDevicesConfigurer(nullptr)
	, mGraphicsWatcherManager(nullptr)
{
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, &mActionsManager, &ActionsManager::onRobotModelChanged);

	mRobotSettingsPage = new ui::RobotsSettingsPage(mKitPluginManager, mRobotModelManager);
}

RobotsPluginFacade::~RobotsPluginFacade()
{
	delete mInterpreter;
	delete mParser;
}

void RobotsPluginFacade::init(qReal::PluginConfigurator const &configurer)
{
	mDevicesConfigurationManager.reset(new DevicesConfigurationManager(
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

	mParser = new textLanguage::RobotsBlockParser(configurer.mainWindowInterpretersInterface().errorReporter()
			, mRobotModelManager
			, [this]() { return mInterpreter ? mInterpreter->timeElapsed() : 0; });


	initSensorWidgets();

	interpreterBase::blocksBase::BlocksFactoryInterface * const coreFactory = new coreBlocks::CoreBlocksFactory();
	coreFactory->configure(configurer.graphicalModelApi()
			, configurer.logicalModelApi()
			, mRobotModelManager
			, *configurer.mainWindowInterpretersInterface().errorReporter()
			, mParser
			);
	mBlocksFactoryManager.addFactory(coreFactory);

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

	connect(&configurer.systemEvents(), &SystemEventsInterface::closedMainWindow
			, mInterpreter, &interpreter::InterpreterInterface::stopRobot);
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, mInterpreter, &interpreter::InterpreterInterface::stopRobot);

	initKitPlugins(configurer);

	auto paletteUpdateManager = new PaletteUpdateManager(configurer.mainWindowInterpretersInterface()
			, mBlocksFactoryManager, this);
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, paletteUpdateManager, &PaletteUpdateManager::updatePalette);
	mDevicesConfigurationManager->connectDevicesConfigurationProvider(interpreter);

	connectInterpreterToActions();

	connectEventsForKitPlugin();

	connect(&mActionsManager.robotSettingsAction(), &QAction::triggered
			, [=] () { configurer.mainWindowInterpretersInterface().openSettingsDialog(tr("Robots")); });
	connect(&configurer.systemEvents(), &SystemEventsInterface::activeTabChanged
			, &mActionsManager, &ActionsManager::onActiveTabChanged);

	sync();
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
	/// @todo: do we need this method?
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
	mDockDevicesConfigurer = new interpreterBase::DevicesConfigurationWidget(nullptr, true);
	mDockDevicesConfigurer->loadRobotModels(mKitPluginManager.allRobotModels());
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, mDockDevicesConfigurer, &interpreterBase::DevicesConfigurationWidget::selectRobotModel);

	mWatchListWindow = new utils::WatchListWindow(mParser);
	mGraphicsWatcherManager = new GraphicsWatcherManager(mParser, this);

	mCustomizer.placeDevicesConfig(mDockDevicesConfigurer);
	mCustomizer.placeWatchPlugins(mWatchListWindow, mGraphicsWatcherManager->widget());

	mDevicesConfigurationManager->connectDevicesConfigurationProvider(mRobotSettingsPage);
	mDevicesConfigurationManager->connectDevicesConfigurationProvider(mDockDevicesConfigurer);
	mDevicesConfigurationManager->connectDevicesConfigurationProvider(mGraphicsWatcherManager);
}

void RobotsPluginFacade::initKitPlugins(qReal::PluginConfigurator const &configurer)
{
	/// @todo: Check that this code works when different kit is selected
	for (QString const &kitId : mKitPluginManager.kitIds()) {
		for (interpreterBase::KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
			kit->init(mEventsForKitPlugin, configurer.systemEvents()
					, configurer.graphicalModelApi(), configurer.logicalModelApi(), *mInterpreter);

			for (interpreterBase::robotModel::RobotModelInterface const *model : kit->robotModels()) {
				initFactoriesFor(kitId, model, configurer);
				connect(&mEventsForKitPlugin, &interpreterBase::EventsForKitPluginInterface::interpretationStarted
						, model, &interpreterBase::robotModel::RobotModelInterface::onInterpretationStarted);
			}

			mDevicesConfigurationManager->connectDevicesConfigurationProvider(kit->devicesConfigurationProvider());
		}

		for (generatorBase::GeneratorKitPluginInterface * const generator : mKitPluginManager.generatorsById(kitId)) {
			generator->init(configurer, mRobotModelManager);
		}
	}
}

void RobotsPluginFacade::initFactoriesFor(QString const &kitId
		, interpreterBase::robotModel::RobotModelInterface const *model
		, PluginConfigurator const &configurer)
{
	// Pulling each robot model to each kit plugin with same ids. We need it for supporting
	// plugin-based blocks set extension for concrete roobt model.
	for (interpreterBase::KitPluginInterface * const kit : mKitPluginManager.kitsById(kitId)) {
		interpreterBase::blocksBase::BlocksFactoryInterface * const factory = kit->blocksFactoryFor(model);
		if (factory) {
			/// @todo Non-obvious dependency on mParser, which may or may not be constructed here.
			///       More functional style will be helpful here.
			factory->configure(configurer.graphicalModelApi()
					, configurer.logicalModelApi()
					, mRobotModelManager
					, *configurer.mainWindowInterpretersInterface().errorReporter()
					, mParser
					);
			mBlocksFactoryManager.addFactory(factory, model);
		}
	}
}

void RobotsPluginFacade::connectEventsForKitPlugin()
{
	QObject::connect(
			mInterpreter
			, &interpreter::InterpreterInterface::started
			, &mEventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::interpretationStarted
			);

	QObject::connect(
			mInterpreter
			, &interpreter::InterpreterInterface::stopped
			, &mEventsForKitPlugin
			, &interpreterBase::EventsForKitPluginInterface::interpretationStopped
			);

	QObject::connect(
			&mRobotModelManager
			, &RobotModelManager::robotModelChanged
			, [this](interpreterBase::robotModel::RobotModelInterface &model) {
				emit mEventsForKitPlugin.robotModelChanged(model.name());
			}
			);

}

void RobotsPluginFacade::sync()
{
	mRobotModelManager.sync();
}
