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
