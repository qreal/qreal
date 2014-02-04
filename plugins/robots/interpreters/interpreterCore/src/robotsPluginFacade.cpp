#include "robotsPluginFacade.h"

using namespace interpreterCore;

RobotsPluginFacade::RobotsPluginFacade()
	: mInterpreter(nullptr)
	, mKitPluginManager("plugins/kitPlugins")
{
	mRobotSettingsPage = new RobotsSettingsPage(mKitPluginManager, mRobotModelManager);

	//	mInterpreter = new details::Interpreter();

	//	connect(mInterpreter, SIGNAL(noiseSettingsChangedBy2DModelWidget()), mRobotSettingsPage, SLOT(rereadNoiseSettings()));
	//	connect(mRobotSettingsPage, SIGNAL(textVisibleChanged(bool)), this, SLOT(titlesVisibilityCheckedInPlugin(bool)));
}

RobotsPluginFacade::~RobotsPluginFacade()
{
	delete mInterpreter;
}

void RobotsPluginFacade::init(qReal::PluginConfigurator const &configurator)
{
	//	details::Tracer::debug(details::tracer::enums::initialization, "RobotsPlugin::init", "Initializing plugin");

	// TODO: reinit it each time when robot model changes
	QString const selectedKit = SettingsManager::value("SelectedRobotKit").toString();
	if (selectedKit.isEmpty() && !mKitPluginManager.kitIds().isEmpty()) {
		SettingsManager::setValue("SelectedRobotKit", mKitPluginManager.kitIds()[0]);
	} else if (mKitPluginManager.kitIds().isEmpty()) {
		configurator.mainWindowInterpretersInterface()->setEnabledForAllElementsInPalette(false);

		// TODO: Correctly handle unselected kit.
		return;
	}

	mKitPluginManager.selectKit(selectedKit);

//	interpreterBase::robotModel::RobotModelInterface * const robotModel = new interpreterBase::robotModel::RobotModel();

//	interpreterBase::blocks::BlocksFactoryInterface * const blocksFactory = new interpreterBase::blocks::BlocksFactory(
//			configurator.graphicalModelApi()
//			, configurator.logicalModelApi()
//			, robotModel
//			, configurator.mainWindowInterpretersInterface().errorReporter()
//			);

//	mInterpreter = new interpreter::Interpreter(configurator.graphicalModelApi()
//			, configurator.logicalModelApi()
//			, configurator.mainWindowInterpretersInterface()
//			, configurator.projectManager()
//			, blocksFactory
//			, robotModel
//			);

//	blocksFactory->setParser(&mInterpreter->parser());

	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
	mSceneCustomizer = &configurator.sceneCustomizer();
	SettingsManager::setValue("IndexGrid", gridWidth);
//	mCustomizer.placeSensorsConfig(produceSensorsConfigurer());
//	mCustomizer.placeWatchPlugins(mInterpreter.watchWindow(), mInterpreter.graphicsWatchWindow());

	initActions();
	initHotKeyActions();

	rereadSettings();

//	setGraphWatcherSettings();
	connect(mRobotSettingsPage, SIGNAL(saved()), this, SLOT(rereadSettings()));
//	connect(mRobotSettingsPage, SIGNAL(saved()), this, SLOT(setGraphWatcherSettings()));
	updateEnabledActions();

//	SystemEventsInterface const *systemEvents = &configurator.systemEvents();

//	connect(systemEvents, SIGNAL(settingsUpdated()), this, SLOT(updateSettings()));
//	connect(systemEvents, SIGNAL(activeTabChanged(Id)), this, SLOT(activeTabChanged(Id)));
//	connect(systemEvents, SIGNAL(closedMainWindow()), this, SLOT(closeNeededWidget()));

//	details::Tracer::debug(details::tracer::enums::initialization, "RobotsPlugin::init", "Initializing done");
}
