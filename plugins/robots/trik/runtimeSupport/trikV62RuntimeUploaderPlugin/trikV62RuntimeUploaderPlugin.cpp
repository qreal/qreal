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
#include <qrkernel/platformInfo.h>

using namespace trik;

#ifdef Q_OS_WIN

const QString createTrikDirectory = "call mkdir -p /home/root/trik";
const QString removePermissions = "call chmod a-x trik/trik*";
const QString killTrikGui = "call killall -q trikGui || :";
const QString moveCommand = "synchronize remote . /home/root/trik";

const QString replaceTrikGuiSh = "call mv /home/root/trik/trikGui.sh /etc/trik/trikGui.sh";
const QString restoreTrikGuiShPermissions = "call chmod a+x /etc/trik/trikGui.sh";

const QString restorePermissions = "call chmod a+x trik/trik*";
const QString restartTrikGui = "call /sbin/reboot";

const QStringList commands = {
		createTrikDirectory
		, removePermissions
		, killTrikGui
		, moveCommand
		, replaceTrikGuiSh
		, restoreTrikGuiShPermissions
		, restorePermissions
		, restartTrikGui
};

#else

const QString preCopyCommand = "ssh -v -oConnectTimeout=%SSH_TIMEOUT%s -oStrictHostKeyChecking=no "
		"-oUserKnownHostsFile=/dev/null root@%IP% \""
		"mkdir -p /home/root/trik; "
		"chmod a-x trik/trik*; "
		"killall -q trikGui"
		"\"";

const QString copyCommand = "scp -r -v -oConnectTimeout=%SSH_TIMEOUT%s -oStrictHostKeyChecking=no "
		"-oUserKnownHostsFile=/dev/null %PATH%/* root@%IP%:/home/root/trik";

const QString postCopyCommand = "ssh -v -oConnectTimeout=%SSH_TIMEOUT%s -oStrictHostKeyChecking=no "
		"-oUserKnownHostsFile=/dev/null root@%IP% \""
		"mv /home/root/trik/trikGui.sh /etc/trik/trikGui.sh; "
		"mv chmod a+x /etc/trik/trikGui.sh; "
		"chmod a+x trik/trik*; "
		"/sbin/reboot"
		"\"";

const QStringList commands = {
		preCopyCommand
		, copyCommand
		, postCopyCommand
};

#endif

TrikV62RuntimeUploaderPlugin::TrikV62RuntimeUploaderPlugin()
	: mUploaderTool(
			tr("Upload Runtime")
			, ":/trik/images/flashRobot.svg"
			, "trikV62Kit"
			, commands
			, QObject::tr("Attention! Started to download the runtime. This can take a minute or two."
					" Please do not turn off the robot.")
			, [](){ return qReal::SettingsManager::value("TrikTcpServer").toString(); }
			)
{
}

void TrikV62RuntimeUploaderPlugin::init(const qReal::PluginConfigurator &configurator)
{
	mUploaderTool.init(configurator.mainWindowInterpretersInterface()
			, qReal::PlatformInfo::invariantSettingsPath("pathToTrikRuntime"));
}

QList<qReal::ActionInfo> TrikV62RuntimeUploaderPlugin::actions()
{
	return {mUploaderTool.action()};
}
