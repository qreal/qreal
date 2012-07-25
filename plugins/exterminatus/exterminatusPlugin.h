#pragma once

#include "../../qrgui/toolPluginInterface/toolPluginInterface.h"

namespace exterminatus {

/// Plugin to exterminate biosphere of the planet
/// and all elements from repository.
class ExterminatusPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)

public:
	ExterminatusPlugin();
	virtual ~ExterminatusPlugin();

	virtual QList<qReal::ActionInfo> actions();

	virtual void init(qReal::PluginConfigurator const &configurator);

private slots:
	/// deletion of all items from repository
	void exterminate();

private:
	qrRepo::RepoControlInterface *mRepo;  // Doesn't have ownership
	QAction mAction;

	/// To syncronize logical and graphical models with repository.
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;

};

}
