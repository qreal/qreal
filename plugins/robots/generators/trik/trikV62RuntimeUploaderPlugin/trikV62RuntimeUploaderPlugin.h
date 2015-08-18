/* Copyright 2007-2015 QReal Research Group, CyberTech Labs Ltd.
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
#include <utils/uploaderTool.h>

namespace trik {

/// Uploader for trikRuntime (for new case).
class TrikV62RuntimeUploaderPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "trik.TrikV62RuntimeUploaderPlugin")

public:
	TrikV62RuntimeUploaderPlugin();

	void init(const qReal::PluginConfigurator &configurator) override;
	QList<qReal::ActionInfo> actions() override;  // Transfers ownership of QAction objects.

private:
	UploaderTool mUploaderTool;
};

}
