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
	QString const openConnection = "\"open scp://root@"
			+ qReal::SettingsManager::value("TrikTcpServer").toString() + "\"" ;
	QString const killTrikGui = "\"call killall trikGui\"";
	QString const restorePermissions = "\"call chmod a+x trik/trik*\"";
	QString const restartTrikGui = "\"call cd trik\" \"call ./trikGui -qws &\"";

	QString const moveCommand = " \"synchronize remote trikRuntime /home/root/trik\"";

	QString const command = "\"" + qReal::SettingsManager::value("WinScpPath").toString() + "\" " // path to WinScp
			"/command " + openConnection + "  " + killTrikGui
			+ " " + moveCommand + " " + restorePermissions + " " + restartTrikGui;

	mMainWindowInterpretersInterface->errorReporter()->addWarning(
		tr("Attention! Start downloading the runtime. Please do not turn off the robot")
	);

	scpProcess.startDetached(command + " \"exit\" ");
}
