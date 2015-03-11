#pragma once

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>

namespace trik {

class TrikRuntimeUploaderPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "trik.TrikRuntimeUploaderPlugin")

public:
	TrikRuntimeUploaderPlugin();

	void init(const qReal::PluginConfigurator &configurator) override;
	QList<qReal::ActionInfo> actions() override;  // Transfers ownership of QAction objects.

private slots:
	void uploadRuntime();

private:
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;  // Does not have ownership
	QAction *mAction;  // Doesn't have ownership; may be disposed by GUI.
};

}
