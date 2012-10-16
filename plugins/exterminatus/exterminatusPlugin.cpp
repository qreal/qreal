#include "exterminatusPlugin.h"

Q_EXPORT_PLUGIN2(exterminatus, exterminatus::ExterminatusPlugin)

using namespace exterminatus;

ExterminatusPlugin::ExterminatusPlugin()
	: mRepo(NULL)
	, mAction(tr("exterminate"), NULL)
{
	connect(&mAction, SIGNAL(triggered()), this, SLOT(exterminate()));
}

ExterminatusPlugin::~ExterminatusPlugin()
{
}

QList<qReal::ActionInfo> ExterminatusPlugin::actions()
{
	qReal::ActionInfo info(&mAction, "file", "tools");
	QList<qReal::ActionInfo> result;
	result << info;
	return result;
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
