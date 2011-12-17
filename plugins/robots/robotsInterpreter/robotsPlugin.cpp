#include "robotsPlugin.h"
#include "details/tracer.h"

#include <QtGui/QApplication>

Q_EXPORT_PLUGIN2(robotsPlugin, qReal::interpreters::robots::RobotsPlugin)

using namespace qReal;
using namespace interpreters::robots;

RobotsPlugin::RobotsPlugin()
		: mMainWindowInterpretersInterface(NULL)
		, m2dModelAction(NULL)
		, mRunAction(NULL)
		, mStopAction(NULL)
{
//	details::Tracer::enableAll();
	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::RobotsPlugin", "Plugin constructor");
	mAppTranslator = new QTranslator();
	mAppTranslator->load(":/robotsInterpreter_" + QLocale::system().name());
	QApplication::installTranslator(mAppTranslator);
}

RobotsPlugin::~RobotsPlugin()
{
	delete mAppTranslator;
}

void RobotsPlugin::init(PluginConfigurator const &configurator)
{
	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::init", "Initializing plugin");
	mInterpreter.init(configurator.graphicalModelApi()
			, configurator.logicalModelApi()
			, configurator.mainWindowInterpretersInterface());
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::init", "Initializing done");
}

qReal::Customizer* RobotsPlugin::customizationInterface()
{
	return &mCustomizer;
}

QList<ActionInfo> RobotsPlugin::actions()
{
	m2dModelAction = new QAction(QObject::tr("2d model"), NULL);
	ActionInfo d2ModelActionInfo(m2dModelAction, "interpreters", "tools");
	QObject::connect(m2dModelAction, SIGNAL(triggered()), this, SLOT(show2dModel()));

	mRunAction = new QAction(QObject::tr("Run"), NULL);
	ActionInfo runActionInfo(mRunAction, "interpreters", "tools");
	QObject::connect(mRunAction, SIGNAL(triggered()), &mInterpreter, SLOT(interpret()));

	mStopAction = new QAction(QObject::tr("Stop"), NULL);
	ActionInfo stopActionInfo(mStopAction, "interpreters", "tools");
	QObject::connect(mStopAction, SIGNAL(triggered()), &mInterpreter, SLOT(stop()));

	QAction *stopRobotAction = new QAction(QObject::tr("Stop robot"), NULL);
	ActionInfo stopRobotActionInfo(stopRobotAction, "interpreters", "tools");
	QObject::connect(stopRobotAction, SIGNAL(triggered()), &mInterpreter, SLOT(stopRobot()));

	QAction *connectToRobotAction = new QAction(QObject::tr("Connect to robot"), NULL);
	connectToRobotAction->setCheckable(true);
	ActionInfo connectToRobotActionInfo(connectToRobotAction, "interpreters", "tools");
	mInterpreter.setConnectRobotAction(connectToRobotAction);
	QObject::connect(connectToRobotAction, SIGNAL(triggered()), &mInterpreter, SLOT(connectToRobot()));

	QAction *robotSettingsAction = new QAction(QObject::tr("Robot settings"), NULL);
	ActionInfo robotSettingsActionInfo(robotSettingsAction, "interpreters", "tools");
	QObject::connect(robotSettingsAction, SIGNAL(triggered()), this, SLOT(showRobotSettings()));

	QAction *watchListAction = new QAction(QObject::tr("Show watch list"), NULL);
	ActionInfo watchListActionInfo(watchListAction, "interpreters", "tools");
	QObject::connect(watchListAction, SIGNAL(triggered()), &mInterpreter, SLOT(showWatchList()));

	QAction *separator = new QAction(NULL);
	ActionInfo separatorActionInfo(separator, "interpreters", "tools");
	separator->setSeparator(true);

	updateSettings();

	return QList<ActionInfo>() << d2ModelActionInfo << runActionInfo << stopActionInfo
			<< stopRobotActionInfo << connectToRobotActionInfo
			<< separatorActionInfo << robotSettingsActionInfo << separatorActionInfo << watchListActionInfo;
}

QPair<QString, PreferencesPage *> RobotsPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Robots"), static_cast<PreferencesPage*>(&mRobotSettingsPage));
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
	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::instance()->value("robotModel", "1").toInt());
	mInterpreter.setRobotModelType(typeOfRobotModel);
	mInterpreter.configureSensors(
			static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType").toInt())
			, static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType").toInt())
			, static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType").toInt())
			, static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType").toInt())
	);
	m2dModelAction->setVisible(typeOfRobotModel == robotModelType::unreal);
	if (typeOfRobotModel == robotModelType::unreal)
		mInterpreter.setD2ModelWidgetActions(mRunAction, mStopAction);
	else
		mInterpreter.showD2ModelWidget(false);

	QString const typeOfCommunication = SettingsManager::value("valueOfCommunication", "bluetooth").toString();
	QString const portName = SettingsManager::value("bluetoothPortName", "").toString();
	mInterpreter.setCommunicator(typeOfCommunication, portName);
	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::updateSettings", "Done updating settings");
}

void RobotsPlugin::closeNeededWidget()
{
	mInterpreter.closeD2ModelWidget();
}
