#pragma once

#include "../../qrgui/toolPluginInterface/toolPluginInterface.h"

namespace exterminatus {

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
	void exterminate();

private:
	qrRepo::RepoControlInterface *mRepo;  // Doesn't have ownership
	QAction mAction;
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;

};

}
