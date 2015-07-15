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
