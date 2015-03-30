#include <qrgui/plugins/interpretedPluginInterface/interpretedPluginInterface.h>

#include "dialogs/specifyGenerationRulesDialog.h"
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>

namespace generationRules {

/// Class, which represents example of using interpreted plugin.
class GenerationRulesPlugin : public QObject, public qReal::InterpretedPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::InterpretedPluginInterface)
	Q_PLUGIN_METADATA(IID "wtfisthis")

public:
	/// Constructor of class GenerationRulesPlugin.
	GenerationRulesPlugin();
	virtual ~GenerationRulesPlugin();

	QList<qReal::ActionInfo> actions() override;
	QList<QAction*> menuActionList() const override;

	void init(const qReal::PluginConfigurator &configurator
			, qrRepo::LogicalRepoApi &metamodelRepoApi
			, qReal::EditorManagerInterface *editorManagerInterface) override;

private slots:
	void generateCode();

	void openGenerationRulesWindow();

private:
	qrRepo::RepoControlInterface *mRepo;  // Doesn't have ownership
	QAction mAction;  // "Generate" button

	/// To syncronize logical and graphical models with repository.
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;  // Doesn't have ownership

	/// Metamodel repo api.
	qrRepo::LogicalRepoApi *mMetamodelRepoApi;  // Doesn't have ownership

	qReal::gui::SpecifyGenerationRulesDialog *mSpecifyGenerationRulesDialog;

	qReal::EditorManagerInterface *mEditorManagerInterface;
};

}
