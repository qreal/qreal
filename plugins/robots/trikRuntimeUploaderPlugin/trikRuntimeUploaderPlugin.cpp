#include "trikRuntimeUploaderPlugin.h"

#include <QtCore/QProcess>
#include <QtWidgets/QApplication>

#include <qrkernel/settingsManager.h>

using namespace trikRuntimeUploaderPlugin;

TrikRuntimeUploaderPlugin::TrikRuntimeUploaderPlugin()
{
	mAppTranslator.load(":/trikRuntimeUploaderPlugin_" + QLocale().name());
	QApplication::installTranslator(&mAppTranslator);

	mAction.reset(new QAction(tr("Upload Runtime"), nullptr));

	connect(mAction.data(), &QAction::triggered, this, &TrikRuntimeUploaderPlugin::uploadRuntime);
}

TrikRuntimeUploaderPlugin::~TrikRuntimeUploaderPlugin()
{
}

QList<qReal::ActionInfo> TrikRuntimeUploaderPlugin::actions()
{
	qReal::ActionInfo info(mAction.data(), "generators", "tools");
	return {info};
}

void TrikRuntimeUploaderPlugin::init(qReal::PluginConfigurator const &configurator)
{
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
}

void TrikRuntimeUploaderPlugin::uploadRuntime()
{
	QProcess scpProcess;
	QString const openConnection = QString("\"open scp://root@%1\"")
			.arg(qReal::SettingsManager::value("TrikTcpServer").toString());

	QString const killTrikGui = "\"call killall trikGui\"";
	QString const restorePermissions = "\"call chmod a+x trik/trik*\"";
	QString const restartTrikGui = "\"call cd trik\" \"call ./trikGui -qws &\"";

	QString const moveCommand = " \"synchronize remote trikRuntime /home/root/trik\"";

	QString const command = QString("\"%1\" /command %2 %3 %4 %5 %6")
			.arg(qReal::SettingsManager::value("WinScpPath").toString())
			.arg(openConnection)
			.arg(killTrikGui)
			.arg(moveCommand)
			.arg(restorePermissions)
			.arg(restartTrikGui);


	if (!scpProcess.startDetached(command + " \"exit\" ")) {
		mMainWindowInterpretersInterface->errorReporter()->addError(
			tr("WinSCP process failed to launch, check path in settings")
		);
	} else {
		mMainWindowInterpretersInterface->errorReporter()->addWarning(
			tr("Attention! Start downloading the runtime. Please do not turn off the robot")
		);
	}
}
