#include "robotsPlugin.h"
#include "details/tracer.h"

#include <QtGui/QApplication>

Q_EXPORT_PLUGIN2(robotsPlugin, qReal::interpreters::robots::RobotsPlugin)

using namespace qReal;
using namespace interpreters::robots;

const Id robotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "RobotsDiagramNode");
const Id oldRobotDiagramType = Id("RobotsMetamodel", "RobotsDiagram", "DiagramNode");

RobotsPlugin::RobotsPlugin()
		: mMainWindowInterpretersInterface(NULL)
		, m2dModelAction(NULL)
		, mRunAction(NULL)
		, mStopRobotAction(NULL)
		, mConnectToRobotAction(NULL)
		, mRobotSettingsAction(NULL)
		, mWatchListAction(NULL)
		, mAppTranslator(new QTranslator())
{
	//details::Tracer::enableCategory(details::tracer::robotCommunication);
	//details::Tracer::enableAll();
	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::RobotsPlugin", "Plugin constructor");
	mAppTranslator->load(":/robotsInterpreter_" + QLocale::system().name());
	QApplication::installTranslator(mAppTranslator);

	mRobotSettingsPage = new PreferencesRobotSettingsPage();

	initActions();

}

RobotsPlugin::~RobotsPlugin()
{
	delete mAppTranslator;
}

void RobotsPlugin::initActions()
{
	m2dModelAction = new QAction(QObject::tr("2d model"), NULL);
	ActionInfo d2ModelActionInfo(m2dModelAction, "interpreters", "tools");
	QObject::connect(m2dModelAction, SIGNAL(triggered()), this, SLOT(show2dModel()));

	mRunAction = new QAction(QObject::tr("Run"), NULL);
	ActionInfo runActionInfo(mRunAction, "interpreters", "tools");
	QObject::connect(mRunAction, SIGNAL(triggered()), &mInterpreter, SLOT(interpret()));

	mStopRobotAction = new QAction(QObject::tr("Stop robot"), NULL);
	ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	QObject::connect(mStopRobotAction, SIGNAL(triggered()), &mInterpreter, SLOT(stopRobot()));

	mConnectToRobotAction = new QAction(QObject::tr("Connect to robot"), NULL);
	mConnectToRobotAction->setCheckable(true);
	ActionInfo connectToRobotActionInfo(mConnectToRobotAction, "interpreters", "tools");
	mInterpreter.setConnectRobotAction(mConnectToRobotAction);
	QObject::connect(mConnectToRobotAction, SIGNAL(triggered()), &mInterpreter, SLOT(connectToRobot()));

	mRobotSettingsAction = new QAction(QObject::tr("Robot settings"), NULL);
	ActionInfo robotSettingsActionInfo(mRobotSettingsAction, "interpreters", "tools");
	QObject::connect(mRobotSettingsAction, SIGNAL(triggered()), this, SLOT(showRobotSettings()));

	mWatchListAction = new QAction(QObject::tr("Show watch list"), NULL);
	ActionInfo watchListActionInfo(mWatchListAction, "interpreters", "tools");
	QObject::connect(mWatchListAction, SIGNAL(triggered()), &mInterpreter, SLOT(showWatchList()));

	QAction *separator = new QAction(NULL);
	ActionInfo separatorActionInfo(separator, "interpreters", "tools");
	separator->setSeparator(true);

	mActionInfos << d2ModelActionInfo << runActionInfo << stopRobotActionInfo
			<< connectToRobotActionInfo << separatorActionInfo << robotSettingsActionInfo
			<< separatorActionInfo << watchListActionInfo;
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
	updateSettings();
	return mActionInfos;
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
	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::instance()->value("robotModel", "1").toInt());
	mInterpreter.setRobotModelType(typeOfRobotModel);
	QString const typeOfCommunication = SettingsManager::value("valueOfCommunication", "bluetooth").toString();
	QString const portName = SettingsManager::value("bluetoothPortName", "").toString();
	mInterpreter.setCommunicator(typeOfCommunication, portName);
	mInterpreter.configureSensors(
			static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType").toInt())
			, static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType").toInt())
			, static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType").toInt())
			, static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType").toInt())
	);
	m2dModelAction->setVisible(typeOfRobotModel == robotModelType::unreal);
	mConnectToRobotAction->setVisible(typeOfRobotModel == robotModelType::real);
	if (typeOfRobotModel == robotModelType::unreal) {
		mInterpreter.setD2ModelWidgetActions(mRunAction, mStopRobotAction);
	} else {
		mInterpreter.showD2ModelWidget(false);
	}

	details::Tracer::debug(details::tracer::initialization, "RobotsPlugin::updateSettings", "Done updating settings");
}

void RobotsPlugin::closeNeededWidget()
{
	mInterpreter.closeD2ModelWidget();
}

void RobotsPlugin::activeTabChanged(Id const & rootElementId)
{
	bool const enabled = rootElementId.type() == robotDiagramType || rootElementId.type() == oldRobotDiagramType;
	foreach (ActionInfo const &actionInfo, mActionInfos) {
		if (needToDisableWhenNotRobotsDiagram(actionInfo.action())) {
			actionInfo.action()->setEnabled(enabled);
		}
	}
	if (enabled) {
		qDebug() << "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo";
		mInterpreter.enableD2ModelWidgetRunStopButtons();
	} else {
		mInterpreter.disableD2ModelWidgetRunStopButtons();
	}
}

bool RobotsPlugin::needToDisableWhenNotRobotsDiagram(QAction const * const action) const
{
	return action != mRobotSettingsAction && action != mConnectToRobotAction && action != m2dModelAction;
}
