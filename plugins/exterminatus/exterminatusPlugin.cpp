#include "exterminatusPlugin.h"

using namespace exterminatus;

ExterminatusPlugin::ExterminatusPlugin()
	: mRepo(nullptr)
	, mAction(tr("exterminate"), nullptr)
{
	connect(&mAction, SIGNAL(triggered()), this, SLOT(exterminate()));
}

ExterminatusPlugin::~ExterminatusPlugin()
{
}

QList<qReal::ActionInfo> ExterminatusPlugin::actions()
{
	qReal::ActionInfo info(&mAction, "file", "tools");
	return {info};
}

void ExterminatusPlugin::init(qReal::PluginConfigurator const &configurator)
{
	mRepo = &configurator.repoControlInterface();
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
}

void ExterminatusPlugin::exterminate()
{
	mRepo->exterminate();
	mMainWindowInterpretersInterface->reinitModels();
}
