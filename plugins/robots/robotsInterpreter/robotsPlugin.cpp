#include <QtWidgets/QApplication>
#include "robotsPlugin.h"
#include "details/tracer.h"
#include "details/nxtDisplay.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details;

Id const robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
Id const oldRobotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "DiagramNode");
int const gridWidth = 25; // Half of element size

RobotsPlugin::RobotsPlugin()
		: mMainWindowInterpretersInterface(NULL)
		, m2dModelAction(NULL)
		, mRunAction(NULL)
		, mStopRobotAction(NULL)
		, mConnectToRobotAction(NULL)
		, mRobotSettingsAction(NULL)
		, mTitlesAction(NULL)
		, mAppTranslator(new QTranslator())
{
	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::RobotsPlugin", "Plugin constructor");
	mAppTranslator->load(":/robotsInterpreter_" + QLocale::system().name());
	QApplication::installTranslator(mAppTranslator);

	mRobotSettingsPage = new PreferencesRobotSettingsPage();

	connect(&mInterpreter, SIGNAL(noiseSettingsChangedBy2DModelWidget()), mRobotSettingsPage, SLOT(rereadNoiseSettings()));

	initActions();
	initHotKeyActions();
}

RobotsPlugin::~RobotsPlugin()
{
	delete mAppTranslator;
}

void RobotsPlugin::initActions()
{
	m2dModelAction = new QAction(QIcon(":/icons/kcron.png"), QObject::tr("2d model"), NULL);
	ActionInfo d2ModelActionInfo(m2dModelAction, "interpreters", "tools");
	QObject::connect(m2dModelAction, SIGNAL(triggered()), this, SLOT(show2dModel()));

	mRunAction = new QAction(QIcon(":/icons/robots_run.png"), QObject::tr("Run"), NULL);
	ActionInfo runActionInfo(mRunAction, "interpreters", "tools");
	QObject::connect(mRunAction, SIGNAL(triggered()), &mInterpreter, SLOT(interpret()));

	mStopRobotAction = new QAction(QIcon(":/icons/robots_stop.png"), QObject::tr("Stop robot"), NULL);
	ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	QObject::connect(mStopRobotAction, SIGNAL(triggered()), &mInterpreter, SLOT(stopRobot()));

	mConnectToRobotAction = new QAction(QIcon(":/icons/robots_connect.png"), QObject::tr("Connect to robot"), NULL);
	mConnectToRobotAction->setCheckable(true);
	ActionInfo connectToRobotActionInfo(mConnectToRobotAction, "interpreters", "tools");
	mInterpreter.setConnectRobotAction(mConnectToRobotAction);
	QObject::connect(mConnectToRobotAction, SIGNAL(triggered()), &mInterpreter, SLOT(connectToRobot()));

	mRobotSettingsAction = new QAction(QIcon(":/icons/robots_settings.png"), QObject::tr("Robot settings"), NULL);
	ActionInfo robotSettingsActionInfo(mRobotSettingsAction, "interpreters", "tools");
	QObject::connect(mRobotSettingsAction, SIGNAL(triggered()), this, SLOT(showRobotSettings()));

	mTitlesAction = new QAction(tr("Text under pictogram"), NULL);
	mTitlesAction->setCheckable(true);
	connect(mTitlesAction, SIGNAL(toggled(bool)), this, SLOT(titlesVisibilityChecked(bool)));
	ActionInfo titlesActionInfo(mTitlesAction, "", "settings");

	QAction *separator = new QAction(NULL);
	ActionInfo separatorActionInfo(separator, "interpreters", "tools");
	separator->setSeparator(true);

	mActionInfos << d2ModelActionInfo << runActionInfo
			<< stopRobotActionInfo << connectToRobotActionInfo
			<< separatorActionInfo << robotSettingsActionInfo
			<< titlesActionInfo;
}

void RobotsPlugin::initHotKeyActions()
{
	mStopRobotAction->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5));
	mRunAction->setShortcut(QKeySequence(Qt::Key_F5));
	m2dModelAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
	mTitlesAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));

	HotKeyActionInfo d2ModelActionInfo("Interpreter.Show2dModel", tr("Show 2d model"), m2dModelAction);
	HotKeyActionInfo runActionInfo("Interpreter.Run", tr("Run interpreter"), mRunAction);
	HotKeyActionInfo stopRobotActionInfo("Interpreter.Stop", tr("Stop interpreter"), mStopRobotAction);
	// TODO: move it into engine
	HotKeyActionInfo titlesActionInfo("Editor.ToggleTitles", tr("Toggle titles visibility"), mTitlesAction);

	mHotKeyActionInfos << d2ModelActionInfo << runActionInfo
			<< stopRobotActionInfo << titlesActionInfo;
}

void RobotsPlugin::init(PluginConfigurator const &configurator)
{
	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::init", "Initializing plugin");
	mInterpreter.init(configurator.graphicalModelApi()
			, configurator.logicalModelApi()
			, configurator.mainWindowInterpretersInterface()
			, configurator.projectManager());
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
	mSceneCustomizer = &configurator.sceneCustomizer();
	SettingsManager::setValue("IndexGrid", gridWidth);
	mCustomizer.placePluginWindows(mInterpreter.watchWindow(), produceSensorsConfigurer());
	rereadSettings();
	connect(mRobotSettingsPage, SIGNAL(saved()), this, SLOT(rereadSettings()));
	updateEnabledActions();
	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::init", "Initializing done");
}

qReal::Customizer* RobotsPlugin::customizationInterface()
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

void RobotsPlugin::show2dModel()
{
	mInterpreter.showD2ModelWidget(true);
}

void RobotsPlugin::updateSettings()
{
	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::updateSettings", "Updating settings, model and sensors are going to be reinitialized...");
	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());
	if (typeOfRobotModel != robotModelType::trik) {
		mInterpreter.setRobotModelType(typeOfRobotModel);
	}

	QString const typeOfCommunication = SettingsManager::value("valueOfCommunication").toString();
	mInterpreter.setCommunicator(typeOfCommunication);
	mInterpreter.configureSensors(
			static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType").toInt())
			, static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType").toInt())
			, static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType").toInt())
			, static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType").toInt())
	);
	m2dModelAction->setVisible(typeOfRobotModel == robotModelType::twoD);
	mConnectToRobotAction->setVisible(typeOfRobotModel == robotModelType::nxt);
	if (typeOfRobotModel == robotModelType::twoD) {
		mInterpreter.setD2ModelWidgetActions(mRunAction, mStopRobotAction);
	} else {
		mInterpreter.showD2ModelWidget(false);
	}

	updateEnabledActions();

	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::updateSettings", "Done updating settings");
}

void RobotsPlugin::closeNeededWidget()
{
	mInterpreter.closeD2ModelWidget();
	mInterpreter.closeWatchList();
}

void RobotsPlugin::activeTabChanged(Id const &rootElementId)
{
	updateEnabledActions();
	bool const enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == oldRobotDiagramType;
	mInterpreter.onTabChanged(rootElementId, enabled);
}

interpreters::robots::details::SensorsConfigurationWidget *RobotsPlugin::produceSensorsConfigurer() const
{
	interpreters::robots::details::SensorsConfigurationWidget *result =
			new interpreters::robots::details::SensorsConfigurationWidget;
	connect(mRobotSettingsPage, SIGNAL(saved()), result, SLOT(refresh()));
	connect(result, SIGNAL(saved()), mRobotSettingsPage, SLOT(refreshPorts()));
	connect(result, SIGNAL(saved()), &mInterpreter, SLOT(saveSensorConfiguration()));
	connect(&mInterpreter, SIGNAL(sensorsConfigurationChanged()), result, SLOT(refresh()));
	mInterpreter.connectSensorConfigurer(result);
	return result;
}

void RobotsPlugin::rereadSettings()
{
	updateTitlesVisibility();
	mInterpreter.setNoiseSettings();
}

void RobotsPlugin::titlesVisibilityChecked(bool checked)
{
	SettingsManager::setValue("showTitlesForRobots", checked);
	updateTitlesVisibility();
}

void RobotsPlugin::updateTitlesVisibility()
{
	bool const titlesVisible = SettingsManager::value("showTitlesForRobots").toBool();
	mSceneCustomizer->setTitlesVisible(titlesVisible);
}

void RobotsPlugin::updateEnabledActions()
{
	Id const &rootElementId = mMainWindowInterpretersInterface->activeDiagram();
	bool const enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == oldRobotDiagramType;

	foreach (ActionInfo const &actionInfo, mActionInfos) {
		actionInfo.action()->setEnabled(enabled);
	}

	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel").toInt());

	mRunAction->setEnabled(typeOfRobotModel != robotModelType::trik && enabled);
	mStopRobotAction->setEnabled(typeOfRobotModel != robotModelType::trik && enabled);
}
