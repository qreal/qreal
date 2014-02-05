#include "robotsPluginFacade.h"

#include "src/coreBlocks/coreBlocksFactory.h"

using namespace interpreterCore;

RobotsPluginFacade::RobotsPluginFacade()
	: mInterpreter(nullptr)
	, mKitPluginManager("plugins/kitPlugins")
	, mActionsManager(mKitPluginManager)
{
	mRobotSettingsPage = new ui::RobotsSettingsPage(mKitPluginManager, mRobotModelManager);
}

RobotsPluginFacade::~RobotsPluginFacade()
{
	delete mInterpreter;
}

void RobotsPluginFacade::init(qReal::PluginConfigurator const &configurer)
{
	mTitlesVisibilityManager.reset(
			new TitlesVisibilityManager(mActionsManager.titlesVisibilityAction(), configurer.sceneCustomizer())
			);

	connect(
			mTitlesVisibilityManager.data()
			, &TitlesVisibilityManager::titlesVisibilityChanged
			, mRobotSettingsPage
			, &ui::RobotsSettingsPage::onTextVisibleChanged
			);

	connect(
			mRobotSettingsPage
			, &ui::RobotsSettingsPage::textVisibleChanged
			, mTitlesVisibilityManager.data()
			, &TitlesVisibilityManager::onTitlesVisibilityChanged
			);

	interpreterBase::blocksBase::BlocksFactoryInterface * const blocksFactory =
			new coreBlocks::CoreBlocksFactory(
//			configurer.graphicalModelApi()
//			, configurer.logicalModelApi()
//			, robotModel
//			, configurer.mainWindowInterpretersInterface().errorReporter()
			);

	mInterpreter = new interpreter::Interpreter(
			configurer.graphicalModelApi()
			, configurer.logicalModelApi()
			, configurer.mainWindowInterpretersInterface()
			, configurer.projectManager()
			, blocksFactory
			, mRobotModelManager
			, mActionsManager.connectToRobotAction()
			);

	blocksFactory->setParser(&mInterpreter->parser());

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

	/// \todo Load currently selected model from registry.
	/// \todo Create default model in case when there is no kit.
	mRobotModelManager.setModel(mKitPluginManager.selectedKit().defaultRobotModel());

	mSensorsConfigurationManager.connectSensorsConfigurationProvider(mRobotSettingsPage);
	// TODO: connect other
	// sensors configurer on dock
	// graphics watcher
	// 2d model;
	// configuration serialization/deserialization
}

PreferencesPage *RobotsPluginFacade::robotsSettingsPage() const
{
	return static_cast<PreferencesPage*>(mRobotSettingsPage);
}

interpreterCore::Customizer &RobotsPluginFacade::customizer()
{
	return mCustomizer;
}

ActionsManager &RobotsPluginFacade::actionsManager()
{
	return mActionsManager;
}
