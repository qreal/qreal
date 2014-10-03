#pragma once

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/plugins/toolPluginInterface/hotKeyActionInfo.h>

#include "ui/robotsSettingsPage.h"

#include "robotsPluginFacade.h"
#include "managers/actionsManager.h"

namespace interpreterCore {

/// Provides entry points to robots plugin. Responsible for implementation of ToolPluginInterface and for
/// interaction with QReal, including ActionInfo objects and other stuff that has nothing to do with plugin logic,
/// but is required by QReal. It also does some basic reaction to actions like "open robot settings page".
class RobotsPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qRealRobots.RobotsPlugin")

public:
	/// Constructor. Creates plugin in uninitialized state, "init" shall be called before plugin can do anything useful.
	RobotsPlugin();

	void init(qReal::PluginConfigurator const &configurator) override;
	qReal::Customizer *customizationInterface() override;  // Does not transfer ownership.
	QPair<QString, qReal::gui::PreferencesPage *> preferencesPage() override;  // Transfers ownership.
	QList<qReal::ActionInfo> actions() override;  // Does not transfer ownership of QAction objects.
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;  // Does not transfer ownership of QAction objects.
	QList<qReal::ProjectConverter> projectConverters() override;
	QStringList defaultSettingsFiles() override;

private:
	/// Main window interface object, used to call GUI functionality.
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;  // Does not have ownership

	QScopedPointer<RobotsPluginFacade> mRobotsPluginFacade;
};

}
