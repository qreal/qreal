#include "robotsPluginFacade.h"

#include "src/coreBlocks/coreBlocksFactory.h"

using namespace interpreterCore;

RobotsPluginFacade::RobotsPluginFacade()
	: mInterpreter(nullptr)
	, mKitPluginManager("plugins/kitPlugins")
	, mActionsManager(mKitPluginManager)
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
	mTitlesVisibilityManager.reset(
			new TitlesVisibilityManager(mActionsManager.titlesVisibilityAction(), configurer.sceneCustomizer())
			);

	interpreterBase::blocksBase::BlocksFactoryInterface * const blocksFactory =
			new coreBlocks::CoreBlocksFactory(
//			configurer.graphicalModelApi()
//			, configurer.logicalModelApi()
//			, robotModel
//			, configurer.mainWindowInterpretersInterface().errorReporter()
			);

	mParser = new textLanguage::RobotsBlockParser(configurer.mainWindowInterpretersInterface().errorReporter());

	mInterpreter = new interpreter::Interpreter(
			configurer.graphicalModelApi()
			, configurer.logicalModelApi()
			, configurer.mainWindowInterpretersInterface()
			, configurer.projectManager()
			, blocksFactory
			, mRobotModelManager
			, *mParser
			, mActionsManager.connectToRobotAction()
			);

	mActionsManager.connectInterpreter(*mInterpreter);

	blocksFactory->setParser(mParser);

	/// \todo reinit it each time when robot model changes
	QString const selectedKit = SettingsManager::value("SelectedRobotKit").toString();
	if (selectedKit.isEmpty() && !mKitPluginManager.kitIds().isEmpty()) {
		SettingsManager::setValue("SelectedRobotKit", mKitPluginManager.kitIds()[0]);
	} else if (mKitPluginManager.kitIds().isEmpty()) {
		configurer.mainWindowInterpretersInterface().setEnabledForAllElementsInPalette(false);

		/// \todo Correctly handle unselected kit.
		return;
	}

	mKitPluginManager.selectKit(selectedKit);

	mDockSensorsConfigurer = new ui::SensorsConfigurationWidget(nullptr, true);
	connect(&mRobotModelManager, &RobotModelManager::robotModelChanged
			, mDockSensorsConfigurer, &ui::SensorsConfigurationWidget::loadRobotModel);

	mWatchListWindow = new utils::WatchListWindow(mParser);
	mGraphicsWatcherManager = new GraphicsWatcherManager(mParser, this);

	mCustomizer.placeSensorsConfig(mDockSensorsConfigurer);
	mCustomizer.placeWatchPlugins(mWatchListWindow, mGraphicsWatcherManager->widget());

	mSensorsConfigurationManager.connectSensorsConfigurationProvider(mRobotSettingsPage);
	mSensorsConfigurationManager.connectSensorsConfigurationProvider(mDockSensorsConfigurer);
	mSensorsConfigurationManager.connectSensorsConfigurationProvider(mGraphicsWatcherManager);
	// TODO: connect 2d model
	// TODO: connect configuration serialization/deserialization into repository

	/// \todo Load currently selected model from registry.
	/// \todo Create default model in case when there is no kit.
	mRobotModelManager.setModel(mKitPluginManager.selectedKit().defaultRobotModel());
}

PreferencesPage *RobotsPluginFacade::robotsSettingsPage() const
{
	return static_cast<PreferencesPage*>(mRobotSettingsPage); // TODO: what for this cast?
}

interpreterCore::Customizer &RobotsPluginFacade::customizer()
{
	return mCustomizer;
}

ActionsManager &RobotsPluginFacade::actionsManager()
{
	return mActionsManager;
}
