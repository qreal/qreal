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

QList<QAction *> GenerationRulesPlugin::menuActionList() const
{
	auto addGenerationRuleAction = new QAction("Add generation rule!!!", nullptr);
	connect(addGenerationRuleAction, &QAction::triggered, this, &GenerationRulesPlugin::openGenerationRulesWindow);

	return {addGenerationRuleAction};
}

void GenerationRulesPlugin::init(
		const qReal::PluginConfigurator &configurator
		, qrRepo::LogicalRepoApi &metamodelRepoApi
		, qReal::EditorManagerInterface *editorManagerInterface)
{
	mRepo = &configurator.repoControlInterface();
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
	mMetamodelRepoApi = &metamodelRepoApi;

	mEditorManagerInterface = editorManagerInterface;
}

void GenerationRulesPlugin::generateCode()
{
	for (int i = 0; i < mMetamodelRepoApi->elementsCount(); ++i) {
		qDebug() << "element";
	}
}

void GenerationRulesPlugin::openGenerationRulesWindow()
{
	const QAction * const action = static_cast<QAction *>(sender());
	const qReal::Id id = action->data().value<qReal::Id>();
	mSpecifyGenerationRulesDialog = new qReal::gui::SpecifyGenerationRulesDialog(mEditorManagerInterface, id);
}
