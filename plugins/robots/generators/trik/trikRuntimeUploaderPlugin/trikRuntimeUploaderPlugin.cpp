#include "trikRuntimeUploaderPlugin.h"

#include <QtCore/QProcess>
#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>

using namespace trik;

TrikRuntimeUploaderPlugin::TrikRuntimeUploaderPlugin()
	: mAction(new QAction(QIcon(":/trik/images/flashRobot.svg"), tr("Upload Runtime"), nullptr))
{
	connect(mAction, &QAction::triggered, this, &TrikRuntimeUploaderPlugin::uploadRuntime);
}

void TrikRuntimeUploaderPlugin::init(const qReal::PluginConfigurator &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
}

QList<qReal::ActionInfo> TrikRuntimeUploaderPlugin::actions()
{
//	QAction *separator = new QAction(this);
//	separator->setSeparator(true);
//	qReal::ActionInfo separatorInfo(separator, "generators", "tools");
	qReal::ActionInfo info(mAction, "generators", "tools");
	return { info/*, separatorInfo*/ };
}

void TrikRuntimeUploaderPlugin::uploadRuntime()
{
	QProcess scpProcess;
	const QString openConnection = QString("\"open scp://root@%1\"")
			.arg(qReal::SettingsManager::value("TrikTcpServer").toString());

	const QString killTrikGui = "\"call killall trikGui\"";
	const QString createTrikDirectory = "\"call mkdir -p /home/root/trik\"";
	const QString removePermissions = "\"call chmod a-x trik/trik*\"";
	const QString restorePermissions = "\"call chmod a+x trik/trik*\"";
	const QString restartTrikGui = "\"call /bin/sh -c '/etc/trik/trikGui.sh &'\"";

	const QString moveCommand = " \"synchronize remote trikRuntime /home/root/trik\"";

	const QString rawWinscpPath = qReal::SettingsManager::value("WinScpPath").toString();
	const QString winscpPath = rawWinscpPath.startsWith("./")
			? QApplication::applicationDirPath() + rawWinscpPath.mid(1)
			: rawWinscpPath;

	const QString command = QString("\"%1\" /command %2 %3 %4 %5 %6 %7 %8")
			.arg(winscpPath)
			.arg(openConnection)
			.arg(createTrikDirectory)
			.arg(removePermissions)
			.arg(killTrikGui)
			.arg(moveCommand)
			.arg(restorePermissions)
			.arg(restartTrikGui);

	if (!scpProcess.startDetached(command + " \"exit\" ")) {
		mMainWindowInterface->errorReporter()->addError(
			tr("WinSCP process failed to launch, check path in settings.")
		);
	} else {
		mMainWindowInterface->errorReporter()->addWarning(
			tr("Attention! Started to download the runtime. Please do not turn off the robot.")
		);
	}
}
