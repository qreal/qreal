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

#include "trikV62RuntimeUploaderPlugin.h"

#include <qrkernel/settingsManager.h>

using namespace trik;

const QString createTrikDirectory = "call mkdir -p /home/root/trik";
const QString removePermissions = "call chmod a-x trik/trik*";
const QString killTrikGui = "call killall -q trikGui || :";
const QString moveCommand = "synchronize remote trikRuntime /home/root/trik";
const QString restorePermissions = "call chmod a+x trik/trik*";
const QString restartTrikGui = "call /bin/sh -c '/etc/trik/trikGui.sh &'";

TrikV62RuntimeUploaderPlugin::TrikV62RuntimeUploaderPlugin()
	: mUploaderTool(
			tr("Upload Runtime")
			, ":/trik/images/flashRobot.svg"
			, "trikV62Kit"
			, {
					createTrikDirectory
					, removePermissions
					, killTrikGui
					, moveCommand
					, restorePermissions
					, restartTrikGui
					}
			, QObject::tr("Attention! Started to download the runtime. This can take a minute or two."
					" Please do not turn off the robot.")
			, [](){ return qReal::SettingsManager::value("TrikTcpServer").toString(); }
			)
{
}

void TrikV62RuntimeUploaderPlugin::init(const qReal::PluginConfigurator &configurator)
{
	mUploaderTool.init(configurator.mainWindowInterpretersInterface());
}

QList<qReal::ActionInfo> TrikV62RuntimeUploaderPlugin::actions()
{
	return {mUploaderTool.action()};
}
