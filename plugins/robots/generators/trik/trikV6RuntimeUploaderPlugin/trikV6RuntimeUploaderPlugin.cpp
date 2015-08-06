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

#include "trikV6RuntimeUploaderPlugin.h"

#include <qrkernel/settingsManager.h>

using namespace trik;

const QString createTrikDirectory = "call mkdir -p /home/root/trik";
const QString removePermissions = "call chmod a-x trik/trik*";
const QString killTrikGui = "call killall -q trikGui || :";
const QString moveCommand = "synchronize remote trikRuntime /home/root/trik";
const QString restorePermissions = "call chmod a+x trik/trik*";

// To make trikRuntime work with old case we use old configs supplied with trikRuntime itself.
const QString replaceSystemConfig = "call mv trik/system-config-v6.xml trik/system-config.xml";
const QString replaceModelConfig = "call mv trik/model-config-v6.xml trik/model-config.xml";

const QString restartTrikGui = "call /bin/sh -c '/etc/trik/trikGui.sh &'";

TrikV6RuntimeUploaderPlugin::TrikV6RuntimeUploaderPlugin()
	: mUploaderTool(
			tr("Upload Runtime (old case)")
			, ":/trik/images/flashRobot.svg"
			, "trikKit"
			, {
					createTrikDirectory
					, removePermissions
					, killTrikGui
					, moveCommand
					, restorePermissions
					, replaceSystemConfig
					, replaceModelConfig
					, restartTrikGui
					}
			, QObject::tr("Attention! Started to download the runtime. This can take a minute or two."
					" Please do not turn off the robot.")
			, [](){ return qReal::SettingsManager::value("TrikTcpServer").toString(); }
			)
{
}

void TrikV6RuntimeUploaderPlugin::init(const qReal::PluginConfigurator &configurator)
{
	mUploaderTool.init(configurator.mainWindowInterpretersInterface());
}

QList<qReal::ActionInfo> TrikV6RuntimeUploaderPlugin::actions()
{
	return {mUploaderTool.action()};
}
