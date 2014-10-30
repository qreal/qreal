#include "generationRulesPlugin.h"

#include <QtWidgets/QAction>

using namespace generationRules;

GenerationRulesPlugin::GenerationRulesPlugin()
	: mRepo(nullptr)
	, mAction(tr("Destroy everything"), nullptr)
{
	connect(&mAction, &QAction::triggered, this, &GenerationRulesPlugin::generateCode);
}

GenerationRulesPlugin::~GenerationRulesPlugin()
{
}

QList<qReal::ActionInfo> GenerationRulesPlugin::actions()
{
	qReal::ActionInfo info(&mAction, "file", "tools");
	return {info};
}

void GenerationRulesPlugin::init(qReal::PluginConfigurator const &configurator, qrRepo::LogicalRepoApi &metamodelRepoApi)
{
	mRepo = &configurator.repoControlInterface();
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
	mMetamodelRepoApi = &metamodelRepoApi;
}

void GenerationRulesPlugin::generateCode()
{
	for (int i = 0; i < mMetamodelRepoApi->elementsCount(); ++i) {
		qDebug() << "element";
	}
}
