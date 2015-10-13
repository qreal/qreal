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

#include <interpreterCore/robotsPluginFacade.h>

namespace robotsPlugin {

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

	void init(const qReal::PluginConfigurator &configurator) override;
	qReal::Customizer *customizationInterface() override;  // Does not transfer ownership.
	QPair<QString, qReal::gui::PreferencesPage *> preferencesPage() override;  // Transfers ownership.
	QList<qReal::ActionInfo> actions() override;  // Does not transfer ownership of QAction objects.
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;  // Does not transfer ownership of QAction objects.
	QList<qReal::ProjectConverter> projectConverters() override;
	QStringList defaultSettingsFiles() override;
	QObject *guiScriptFacade() override;

private:
	/// Main window interface object, used to call GUI functionality.
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterpretersInterface;  // Does not have ownership

	QScopedPointer<interpreterCore::RobotsPluginFacade> mRobotsPluginFacade;
};

}
