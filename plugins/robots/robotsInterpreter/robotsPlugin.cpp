#include "robotsPlugin.h"

#include <QtCore/QTranslator>
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
	QTranslator *appTranslator = new QTranslator();
	appTranslator->load(":/robotsInterpreter_" + QLocale::system().name());
	QApplication::installTranslator(appTranslator);
}

RobotsPlugin::~RobotsPlugin()
{
}

void RobotsPlugin::init(PluginConfigurator const &configurator)
{
	mInterpreter.init(configurator.graphicalModelApi()
			, configurator.logicalModelApi()
			, configurator.mainWindowInterpretersInterface());
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
}

qReal::CustomizationInterface* RobotsPlugin::customizationInterface()
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
	ActionInfo connectToRobotActionInfo(connectToRobotAction, "interpreters", "tools");
	QObject::connect(connectToRobotAction, SIGNAL(triggered()), &mInterpreter, SLOT(connectToRobot()));

	QAction *robotSettingsAction = new QAction(QObject::tr("Robot settings"), NULL);
	ActionInfo robotSettingsActionInfo(robotSettingsAction, "interpreters", "tools");
	QObject::connect(robotSettingsAction, SIGNAL(triggered()), this, SLOT(showRobotSettings()));

	QAction *separator = new QAction(NULL);
	ActionInfo separatorActionInfo(separator, "interpreters", "tools");
	separator->setSeparator(true);

	updateSettings();

	return QList<ActionInfo>() << d2ModelActionInfo << runActionInfo << stopActionInfo
			<< stopRobotActionInfo << connectToRobotActionInfo
			<< separatorActionInfo << robotSettingsActionInfo;
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
}
