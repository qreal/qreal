#pragma once

#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/plugins/toolPluginInterface/hotKeyActionInfo.h>
#include <qrutils/interpreter/interpreter.h>
#include <qrtext/include/qrtext/lua/luaToolbox.h>

namespace deployment {

class BlocksTable;

/// Provides entry points to robots plugin. Responsible for implementation of ToolPluginInterface and for
/// interaction with QReal, including ActionInfo objects and other stuff that has nothing to do with plugin logic,
/// but is required by QReal. It also does some basic reaction to actions like "open robot settings page".
class DeploymentInterpreterPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qRealRobots.DeploymentInterpreterPlugin")

public:
	/// Constructor. Creates plugin in uninitialized state, "init" shall be called before plugin can do anything useful.
	DeploymentInterpreterPlugin();
	~DeploymentInterpreterPlugin();

	void init(qReal::PluginConfigurator const &configurator) override;
	QList<qReal::ActionInfo> actions() override;  // Does not transfer ownership of QAction objects.
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;  // Does not transfer ownership of QAction objects.

private:
	void placeShellWidget(QWidget *shell, QWidget *mainWindow);

	qrtext::lua::LuaToolbox mTextLanguage;
	/// Main window interface object, used to call GUI functionality.
	BlocksTable *mBlocksTable;  // Takes ownership
	qReal::interpretation::Interpreter *mInterpreter;  // Takes ownership

	QAction *mRunAction;  // Transfers ownership to QObject parent-child system
	QAction *mStopAction;  // Transfers ownership to QObject parent-child system
	QWidget *mShellDock;  // Transfers ownership to QObject parent-child system
};

}
