#include "robotsPluginFacade.h"

#include "src/coreBlocks/coreBlocksFactory.h"

using namespace interpreterCore;

RobotsPluginFacade::RobotsPluginFacade()
	: mInterpreter(nullptr)
	, mKitPluginManager("plugins/kitPlugins")
{
	mRobotSettingsPage = new RobotsSettingsPage(mKitPluginManager, mRobotModelManager);
}

RobotsPluginFacade::~RobotsPluginFacade()
{
	delete mInterpreter;
}

void RobotsPluginFacade::init(qReal::PluginConfigurator const &configurer, QAction &connectToRobotAction)
{
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
			, connectToRobotAction
			);

	blocksFactory->setParser(&mInterpreter->parser());
}

interpreter::InterpreterInterface &RobotsPluginFacade::interpreter()
{
	return *mInterpreter;
}

PreferencesPage *RobotsPluginFacade::robotsSettingsPage() const
{
	return static_cast<PreferencesPage*>(mRobotSettingsPage);
}

KitPluginManager &RobotsPluginFacade::kitPluginManager()
{
	return mKitPluginManager;
}
