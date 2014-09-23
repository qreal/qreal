#pragma once

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>

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

	QStringList defaultSettingsFiles() override;

private slots:
	void checkForUpdates(bool reportNoUpdates = true);
	void showUpdatesDialog();
	void reportNoUpdates();

private:
	void initSettingsUi(qReal::gui::PreferencesPage &behaviourPage);

	QAction mSeparator;
	QAction mCheckForUpdatesAction;
	ErrorReporterInterface *mErrorReporter;
	QWidget *mMainWindowWidget;
};

}
