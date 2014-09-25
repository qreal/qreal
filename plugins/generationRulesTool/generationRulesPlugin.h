#include "../../qrgui/interpretedPluginInterface/interpretedPluginInterface.h"

namespace generationRules {
class GenerationRulesPlugin : public QObject, public qReal::InterpretedPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::InterpretedPluginInterface)
	Q_PLUGIN_METADATA(IID "wtfisthis")

public:
	GenerationRulesPlugin();
	virtual ~GenerationRulesPlugin();

	virtual QList<qReal::ActionInfo> actions();

	virtual void init(qReal::PluginConfigurator const &configurator, qrRepo::LogicalRepoApi &metamodelRepoApi);

private slots:

	void generateCode();

private:
	qrRepo::RepoControlInterface *mRepo; // Doesn't have ownership
	QAction mAction; // "Generate" button

	/// To syncronize logical and graphical models with repository.
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;

	/// Metamodel repo api.
	qrRepo::LogicalRepoApi *mMetamodelRepoApi;
};

}
