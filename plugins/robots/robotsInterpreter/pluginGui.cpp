#include "pluginGui.h"

using namespace qReal;
using namespace interpreters::robots;

PluginGui::PluginGui()
	: mMainWindowInterpretersInterface(NULL)
	, m2dModelAction(NULL)
	, mRunAction(NULL)
	, mStopAction(NULL)
{
}

QList<CustomToolInterface::ActionInfo> PluginGui::actions()
{
	// TODO: Move this to separate classes
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

void PluginGui::init(PluginConfigurator const &configurator)
{
	mInterpreter.init(configurator.graphicalModelApi()
			, configurator.logicalModelApi()
			, configurator.mainWindowInterpretersInterface());
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
}

QPair<QString, PreferencesPage *> PluginGui::preferencesPage()
{
	return qMakePair(QObject::tr("Robots"), static_cast<PreferencesPage*>(&mRobotSettinsPage));
}

void PluginGui::showRobotSettings()
{
	mMainWindowInterpretersInterface->openSettingsDialog(tr("Robots"));
}

void PluginGui::show2dModel()
{
	mInterpreter.showD2ModelWidget(true);
}

void PluginGui::updateSettings()
{
	QString const bluetoothPortName = SettingsManager::instance()->value("bluetoothPortName").toString();
	mInterpreter.setBluetoothPortName(bluetoothPortName);
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
}
