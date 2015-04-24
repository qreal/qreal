/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
