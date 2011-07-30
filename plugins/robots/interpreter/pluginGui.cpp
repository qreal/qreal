#include "pluginGui.h"

using namespace qReal;
using namespace interpreters::robots;

QList<CustomToolInterface::ActionInfo> PluginGui::actions()
{
	// TODO: Move this to separate classes
	QAction *d2ModelAction = new QAction(QObject::tr("2d model"), NULL);
	ActionInfo d2ModelActionInfo(d2ModelAction, "interpreters", "tools");
	QObject::connect(d2ModelAction, SIGNAL(triggered()), this, SLOT(show2dModel()));

	QAction *runAction = new QAction(QObject::tr("Run"), NULL);
	ActionInfo runActionInfo(runAction, "interpreters", "tools");
	QObject::connect(runAction, SIGNAL(triggered()), &mInterpreter, SLOT(interpret()));

	QAction *stopAction = new QAction(QObject::tr("Stop"), NULL);
	ActionInfo stopActionInfo(stopAction, "interpreters", "tools");
	QObject::connect(stopAction, SIGNAL(triggered()), &mInterpreter, SLOT(stop()));

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
