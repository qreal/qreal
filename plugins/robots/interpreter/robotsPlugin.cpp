#include "robotsPlugin.h"

using namespace robots;

RobotsPlugin::RobotsPlugin()
{
}

RobotsPlugin::~RobotsPlugin()
{
}

void RobotsPlugin::initPlugin(qrRepo::RepoControlInterface *repo)
{
	Q_UNUSED(repo);
}

QList<qReal::CustomToolInterface *> RobotsPlugin::toolInterfaces()
{
	return QList<qReal::CustomToolInterface *>();
}

QList<qReal::InterpreterInterface *> RobotsPlugin::interpreterInterfaces()
{
	return QList<qReal::InterpreterInterface *>();
}
