#include "exterminatusPlugin.h"

using namespace exterminatus;

ExterminatusPlugin::ExterminatusPlugin()
	: mRepo(nullptr)
	, mAction(tr("exterminate"), nullptr)
{
	connect(&mAction, &QAction::triggered, this, &ExterminatusPlugin::exterminate);
}

ExterminatusPlugin::~ExterminatusPlugin()
{
}

QList<qReal::ActionInfo> ExterminatusPlugin::actions()
{
	return { qReal::ActionInfo(&mAction, "file", "tools") };
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
