#include "robotsPlugin.h"

#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>

//#include "details/tracer.h"
//#include "details/nxtDisplay.h"

//#include <qrgui/toolPluginInterface/systemEvents.h>

using namespace qReal;
using namespace interpreterCore;

Id const robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
Id const subprogramDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "SubprogramDiagram");
int const gridWidth = 25; // Half of element size

RobotsPlugin::RobotsPlugin()
		: mInterpreter(nullptr)
		, mMainWindowInterpretersInterface(nullptr)
//		, m2dModelAction(nullptr)
		, mRunAction(nullptr)
		, mStopRobotAction(nullptr)
		, mConnectToRobotAction(nullptr)
		, mRobotSettingsAction(nullptr)
		, mTitlesAction(nullptr)
		, mAppTranslator(new QTranslator())
		, mKitPluginManager("plugins/kitPlugins")
{
//	details::Tracer::debug(details::tracer::enums::initialization, "RobotsPlugin::RobotsPlugin", "Plugin constructor");
	mAppTranslator->load(":/robotsInterpreter_" + QLocale::system().name());
	QApplication::installTranslator(mAppTranslator);

//	mInterpreter = new details::Interpreter();

	mRobotSettingsPage = new RobotsSettingsPage(mKitPluginManager);

//	connect(mInterpreter, SIGNAL(noiseSettingsChangedBy2DModelWidget()), mRobotSettingsPage, SLOT(rereadNoiseSettings()));
//	connect(mRobotSettingsPage, SIGNAL(textVisibleChanged(bool)), this, SLOT(titlesVisibilityCheckedInPlugin(bool)));
}

RobotsPlugin::~RobotsPlugin()
{
	delete mAppTranslator;
	delete mInterpreter;
}

void RobotsPlugin::initActions()
{
//	m2dModelAction = new QAction(QIcon(":/icons/2d-model.svg"), QObject::tr("2d model"), NULL);
//	ActionInfo d2ModelActionInfo(m2dModelAction, "interpreters", "tools");
//	QObject::connect(m2dModelAction, SIGNAL(triggered()), this, SLOT(show2dModel()));

	mRunAction = new QAction(QIcon(":/icons/robots_run.png"), QObject::tr("Run"), nullptr);
	ActionInfo runActionInfo(mRunAction, "interpreters", "tools");
	QObject::connect(mRunAction, SIGNAL(triggered()), mInterpreter, SLOT(interpret()));

	mStopRobotAction = new QAction(QIcon(":/icons/robots_stop.png"), QObject::tr("Stop robot"), nullptr);
	ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	QObject::connect(mStopRobotAction, SIGNAL(triggered()), mInterpreter, SLOT(stopRobot()));

	mConnectToRobotAction = new QAction(QIcon(":/icons/robots_connect.png"), QObject::tr("Connect to robot"), NULL);
	mConnectToRobotAction->setCheckable(true);
	ActionInfo connectToRobotActionInfo(mConnectToRobotAction, "interpreters", "tools");
//	mInterpreter->setConnectRobotAction(mConnectToRobotAction);
	QObject::connect(mConnectToRobotAction, SIGNAL(triggered()), mInterpreter, SLOT(connectToRobot()));

	QAction * const separator1 = new QAction(nullptr);
	separator1->setSeparator(true);
	ActionInfo separator1ActionInfo(separator1, "interpreters", "tools");

	mSwitchTo2DModelAction = new QAction(QIcon(":/icons/switch-2d.svg")
			, QObject::tr("Switch to 2d model"), nullptr);
	mSwitchTo2DModelAction->setCheckable(true);
	ActionInfo switchTo2DModelActionInfo(mSwitchTo2DModelAction, "interpreters", "tools");

	mSwitchToNxtModelAction = new QAction(QIcon(":/icons/switch-real-nxt.svg")
			, QObject::tr("Switch to Lego NXT mode"), nullptr);
	mSwitchToNxtModelAction->setCheckable(true);
	ActionInfo switchToNxtModelActionInfo(mSwitchToNxtModelAction, "interpreters", "tools");

	mSwitchToTrikModelAction = new QAction(QIcon(":/icons/switch-real-trik.svg")
			, QObject::tr("Switch to TRIK mode"), nullptr);
	mSwitchToTrikModelAction->setCheckable(true);
//	mSwitchToTrikModelAction->setVisible(enableTrik);
	ActionInfo switchToTrikModelActionInfo(mSwitchToTrikModelAction, "interpreters", "tools");

	QSignalMapper * const modelTypeMapper = new QSignalMapper(this);
//	modelTypeMapper->setMapping(mSwitchTo2DModelAction, enums::robotModelType::twoD);
//	modelTypeMapper->setMapping(mSwitchToNxtModelAction, enums::robotModelType::nxt);
//	modelTypeMapper->setMapping(mSwitchToTrikModelAction, enums::robotModelType::trik);
//	connect(modelTypeMapper, SIGNAL(mapped(int)), this, SLOT(setModelType(int)));
	connect(mSwitchTo2DModelAction, SIGNAL(triggered()), modelTypeMapper, SLOT(map()));
	connect(mSwitchToNxtModelAction, SIGNAL(triggered()), modelTypeMapper, SLOT(map()));
	connect(mSwitchToTrikModelAction, SIGNAL(triggered()), modelTypeMapper, SLOT(map()));

	QAction * const separator2 = new QAction(nullptr);
	ActionInfo separator2ActionInfo(separator2, "interpreters", "tools");
	separator2->setSeparator(true);

	mRobotSettingsAction = new QAction(QIcon(":/icons/robots_settings.png")
			, QObject::tr("Robot settings"), nullptr);
	ActionInfo robotSettingsActionInfo(mRobotSettingsAction, "interpreters", "tools");
	QObject::connect(mRobotSettingsAction, SIGNAL(triggered()), this, SLOT(showRobotSettings()));

	mTitlesAction = new QAction(tr("Text under pictogram"), nullptr);
	mTitlesAction->setCheckable(true);
	mTitlesAction->setChecked(SettingsManager::value("showTitlesForRobots").toBool());
	connect(mTitlesAction, SIGNAL(toggled(bool)), this, SLOT(titlesVisibilityCheckedInPlugin(bool)));
	ActionInfo titlesActionInfo(mTitlesAction, "", "settings");

	mActionInfos
//			<< d2ModelActionInfo
			<< connectToRobotActionInfo
			<< runActionInfo
			<< stopRobotActionInfo
			<< separator1ActionInfo
			<< switchTo2DModelActionInfo
			<< switchToNxtModelActionInfo
			<< switchToTrikModelActionInfo
			<< separator2ActionInfo
			<< robotSettingsActionInfo
			<< titlesActionInfo
			;
}

void RobotsPlugin::initHotKeyActions()
{
	mStopRobotAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	mRunAction->setShortcut(QKeySequence(Qt::Key_F5));
//	m2dModelAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
	mTitlesAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));

//	HotKeyActionInfo d2ModelActionInfo("Interpreter.Show2dModel", tr("Show 2d model"), m2dModelAction);
	HotKeyActionInfo runActionInfo("Interpreter.Run", tr("Run interpreter"), mRunAction);
	HotKeyActionInfo stopRobotActionInfo("Interpreter.Stop", tr("Stop interpreter"), mStopRobotAction);
	// TODO: move it into engine
	HotKeyActionInfo titlesActionInfo("Editor.ToggleTitles", tr("Toggle titles visibility"), mTitlesAction);

	mHotKeyActionInfos
//			<< d2ModelActionInfo
			<< runActionInfo
			<< stopRobotActionInfo
			<< titlesActionInfo
			;
}

void RobotsPlugin::init(PluginConfigurator const &configurator)
{
	if (SettingsManager::value("kitId").toString().isEmpty() && !mKitPluginManager.kitIds().isEmpty()) {
		SettingsManager::setValue("kitId", mKitPluginManager.kitIds()[0]);
	} else if (mKitPluginManager.kitIds().isEmpty()) {
		mMainWindowInterpretersInterface->setEnabledForAllElementsInPalette(false);

		// TODO: Correctly handle unselected kit.
		return;
	}

	mKitPluginManager.selectKit(SettingsManager::value("kitId").toString());

	//	details::Tracer::debug(details::tracer::enums::initialization, "RobotsPlugin::init", "Initializing plugin");
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

qReal::Customizer *RobotsPlugin::customizationInterface()
{
	return &mCustomizer;
}

QList<ActionInfo> RobotsPlugin::actions()
{
	updateSettings();
	return mActionInfos;
}

QList<HotKeyActionInfo> RobotsPlugin::hotKeyActions()
{
	return mHotKeyActionInfos;
}

QPair<QString, PreferencesPage *> RobotsPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Robots"), static_cast<PreferencesPage*>(mRobotSettingsPage));
}

void RobotsPlugin::showRobotSettings()
{
	mMainWindowInterpretersInterface->openSettingsDialog(tr("Robots"));
}

//void RobotsPlugin::show2dModel()
//{
//	mInterpreter.showD2ModelWidget(true);
//}

void RobotsPlugin::updateSettings()
{
//	details::Tracer::debug(
//			details::tracer::enums::initialization
//			, "RobotsPlugin::updateSettings", "Updating settings, model and sensors are going to be reinitialized..."
//			);

//	robots::enums::robotModelType::robotModelTypeEnum typeOfRobotModel
//			= static_cast<robots::enums::robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());
//	if (typeOfRobotModel != robots::enums::robotModelType::trik) {
//		mInterpreter.setRobotModelType(typeOfRobotModel);
//	}

//	QString const typeOfCommunication = SettingsManager::value("valueOfCommunication").toString();
//	mInterpreter.setCommunicator(typeOfCommunication);
//	mInterpreter.configureSensors(
//			static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType").toInt())
//			, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType").toInt())
//			, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType").toInt())
//			, static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType").toInt())
//	);
//	m2dModelAction->setVisible(typeOfRobotModel == robots::enums::robotModelType::twoD);
//	mConnectToRobotAction->setVisible(typeOfRobotModel == robots::enums::robotModelType::nxt);
//	if (typeOfRobotModel == robots::enums::robotModelType::twoD) {
//		mInterpreter.setD2ModelWidgetActions(mRunAction, mStopRobotAction);
//	} else {
//		mInterpreter.showD2ModelWidget(false);
//	}

//	updateEnabledActions();

//	details::Tracer::debug(details::tracer::enums::initialization, "RobotsPlugin::updateSettings", "Done updating settings");
}

void RobotsPlugin::closeNeededWidget()
{
//	mInterpreter.stopRobot();
//	mInterpreter.closeD2ModelWidget();
//	mInterpreter.closeWatchList();
}

void RobotsPlugin::activeTabChanged(Id const &rootElementId)
{
	updateEnabledActions();
//	bool const enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;
//	mInterpreter.onTabChanged(rootElementId, enabled);
}

//interpreters::robots::details::SensorsConfigurationWidget *RobotsPlugin::produceSensorsConfigurer() const
//{
//	interpreters::robots::details::SensorsConfigurationWidget *result =
//			new interpreters::robots::details::SensorsConfigurationWidget;
//	connect(mRobotSettingsPage, SIGNAL(saved()), result, SLOT(refresh()));
//	connect(result, SIGNAL(saved()), mRobotSettingsPage, SLOT(refreshPorts()));
//	connect(result, SIGNAL(saved()), &mInterpreter, SLOT(saveSensorConfiguration()));
//	connect(&mInterpreter, SIGNAL(sensorsConfigurationChanged()), result, SLOT(refresh()));
//	connect(result, SIGNAL(saved()), &mInterpreter, SLOT(updateGraphicWatchSensorsList()));
//	mInterpreter.connectSensorConfigurer(result);
//	return result;
//}

void RobotsPlugin::rereadSettings()
{
	updateTitlesVisibility();
//	mInterpreter.setNoiseSettings();
	updateBlocksOnPalette();
//	reinitModelType();
}

//void RobotsPlugin::setModelType(int type)
//{
//	SettingsManager::setValue("robotModel", type);
//	reinitModelType();
//	updateSettings();
//}

//void RobotsPlugin::reinitModelType()
//{
//	mSwitchTo2DModelAction->setChecked(false);
//	mSwitchToNxtModelAction->setChecked(false);
//	mSwitchToTrikModelAction->setChecked(false);

//	// Reinitting model type...
//	enums::robotModelType::robotModelTypeEnum typeOfRobotModel =
//			static_cast<enums::robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());
//	switch (typeOfRobotModel) {
//	case enums::robotModelType::twoD:
//		mSwitchTo2DModelAction->setChecked(true);
//		break;
//	case enums::robotModelType::nxt:
//		mSwitchToNxtModelAction->setChecked(true);
//		break;
//	case enums::robotModelType::trik:
//		mSwitchToTrikModelAction->setChecked(true);
//		break;
//	default:
//		break;
//	}
//}

//void RobotsPlugin::setGraphWatcherSettings()
//{
//	mInterpreter.graphicsWatchWindow()->configureUpdateIntervals(
//			SettingsManager::value("sensorUpdateInterval"
//					, utils::sensorsGraph::SensorsGraph::readSensorDefaultInterval).toInt()
//			, SettingsManager::value("autoscalingInterval"
//					, utils::sensorsGraph::SensorsGraph::autoscalingDefault).toInt()
//			, SettingsManager::value("textUpdateInterval"
//					, utils::sensorsGraph::SensorsGraph::textUpdateDefault).toInt()
//	);
//}

void RobotsPlugin::titlesVisibilityChecked(bool checked)
{
	mTitlesAction->setChecked(checked);
}

void RobotsPlugin::titlesVisibilityCheckedInPlugin(bool checked)
{
	titlesVisibilityChecked(checked);
	SettingsManager::setValue("showTitlesForRobots", checked);
	updateTitlesVisibility();
//	mRobotSettingsPage->changeTextVisibleOnSettingPage(checked);
}

void RobotsPlugin::updateTitlesVisibility()
{
	bool const titlesVisible = SettingsManager::value("showTitlesForRobots").toBool();
	mSceneCustomizer->setTitlesVisible(titlesVisible);
}

void RobotsPlugin::updateEnabledActions()
{
	Id const &rootElementId = mMainWindowInterpretersInterface->activeDiagram();
	bool const enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == subprogramDiagramType;

	foreach (ActionInfo const &actionInfo, mActionInfos) {
		actionInfo.action()->setEnabled(enabled);
	}

//	robots::enums::robotModelType::robotModelTypeEnum typeOfRobotModel
//			= static_cast<robots::enums::robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());

//	mRunAction->setEnabled(typeOfRobotModel != robots::enums::robotModelType::trik && enabled);
//	mStopRobotAction->setEnabled(typeOfRobotModel != robots::enums::robotModelType::trik && enabled);
}

void RobotsPlugin::updateBlocksOnPalette()
{
//	mMainWindowInterpretersInterface->setVisibleForAllElementsInPalette(false);
//	IdList const commonBlocks = mInterpreter->providedBlocks();
//	foreach (Id const &id, commonBlocks) {
//		mMainWindowInterpretersInterface->setElementInPaletteVisible(id, true);
//	}

//	IdList const unsupportedBlocks = mKitPluginManager.selectedKit().unsupportedBlocks();
//	foreach (Id const &id, unsupportedBlocks) {
//		mMainWindowInterpretersInterface->setElementInPaletteEnabled(id, false);
//	}
}
