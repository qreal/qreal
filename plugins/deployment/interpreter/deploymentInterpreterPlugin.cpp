#include "deploymentInterpreterPlugin.h"

using namespace qReal;
using namespace deployment;

DeploymentInterpreterPlugin::DeploymentInterpreterPlugin()
	: mMainWindowInterpretersInterface(nullptr)
	, mRunAction(new QAction(QIcon(":/images/run_deployment.png"), QObject::tr("Run deployment"), nullptr))
	, mStopAction(new QAction(QIcon(":/images/stop_deployment.png"), QObject::tr("Stop deployment"), nullptr))
{
}

void DeploymentInterpreterPlugin::init(PluginConfigurator const &configurator)
{
}

QList<ActionInfo> DeploymentInterpreterPlugin::actions()
{
	return { qReal::ActionInfo(mRunAction, "interpreters", "tools")
			, qReal::ActionInfo(mStopAction, "interpreters", "tools")
	};
}

QList<HotKeyActionInfo> DeploymentInterpreterPlugin::hotKeyActions()
{
	return { qReal::HotKeyActionInfo("DeploymentInterpreter.Run", QObject::tr("Run deployment"), mRunAction)
		, qReal::HotKeyActionInfo("DeploymentInterpreter.Stop", QObject::tr("Stop deployment"), mStopAction)
	};
}
