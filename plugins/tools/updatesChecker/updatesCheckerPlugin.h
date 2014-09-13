#pragma once

#include <qrgui/toolPluginInterface/toolPluginInterface.h>

namespace updatesChecker {

/// Checks for new updates on the remote server in background.
class UpdatesCheckerPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "updatesChecker.UpdatesCheckerPlugin")

public:
	UpdatesCheckerPlugin();

	QList<qReal::ActionInfo> actions() override;

	void init(qReal::PluginConfigurator const &configurator) override;
};

}
