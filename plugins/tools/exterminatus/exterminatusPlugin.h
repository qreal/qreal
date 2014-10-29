#pragma once

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>

namespace exterminatus {

/// Plugin to exterminate biosphere of the planet and all elements from repository.
class ExterminatusPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "exterminatus.ExterminatusPlugin")

public:
	ExterminatusPlugin();
	~ExterminatusPlugin() override;

	QList<qReal::ActionInfo> actions() override;

	void init(qReal::PluginConfigurator const &configurator) override;

private slots:
	/// Deletion of all items from repository.
	void exterminate();

private:
	qrRepo::RepoControlInterface *mRepo;  // Doesn't have ownership
	QAction mAction;

	/// To syncronize logical and graphical models with repository.
	/// Doesn`t have ownership.
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;

};

}
