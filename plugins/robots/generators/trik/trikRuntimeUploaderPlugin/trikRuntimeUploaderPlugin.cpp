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

QList<qReal::ActionInfo> TrikRuntimeUploaderPlugin::actions()
{
	QAction *separator = new QAction(this);
	separator->setSeparator(true);
	qReal::ActionInfo separatorInfo(separator, "generators", "tools");
	qReal::ActionInfo info(mAction, "generators", "tools");
	return { info, separatorInfo };
}

void TrikRuntimeUploaderPlugin::uploadRuntime()
{
	QProcess scpProcess;
	QString const openConnection = QString("\"open scp://root@%1\"")
			.arg(qReal::SettingsManager::value("TrikTcpServer").toString());

	QString const killTrikGui = "\"call killall trikGui\"";
	QString const createTrikDirectory = "\"call mkdir -p /home/root/trik\"";
	QString const removePermissions = "\"call chmod a-x trik/trik*\"";
	QString const restorePermissions = "\"call chmod a+x trik/trik*\"";
	QString const restartTrikGui = "\"call /bin/sh -c '/etc/trik/trikGui.sh &'\"";

	QString const moveCommand = " \"synchronize remote trikRuntime /home/root/trik\"";

	QString const rawWinscpPath = qReal::SettingsManager::value("WinScpPath").toString();
	QString const winscpPath = rawWinscpPath.startsWith("./")
			? QApplication::applicationDirPath() + rawWinscpPath.mid(1)
			: rawWinscpPath;

	QString const command = QString("\"%1\" /command %2 %3 %4 %5 %6 %7 %8")
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

generatorBase::MasterGeneratorBase *TrikRuntimeUploaderPlugin::masterGenerator()
{
	return nullptr;
}
