#include "trikRuntimeUploaderPlugin.h"

#include <qrkernel/settingsManager.h>
#include <QtCore/QProcess>

using namespace trikRuntimeUploaderPlugin;

TrikRuntimeUploaderPlugin::TrikRuntimeUploaderPlugin():
		mAction(tr("Upload Runtime"), NULL)
{
	connect(&mAction, SIGNAL(triggered()), this, SLOT(uploadRuntime()));
}

TrikRuntimeUploaderPlugin::~TrikRuntimeUploaderPlugin()
{
}

QList<qReal::ActionInfo> TrikRuntimeUploaderPlugin::actions()
{
	qReal::ActionInfo info(&mAction, "generators", "tools");
	QList<qReal::ActionInfo> result;
	result << info;
	return result;
}

void TrikRuntimeUploaderPlugin::init(qReal::PluginConfigurator const &configurator)
{
	mMainWindowInterpretersInterface = &configurator.mainWindowInterpretersInterface();
}

void TrikRuntimeUploaderPlugin::uploadRuntime()
{
	QProcess myProcess;
	QString openConnection = "\"open scp://root@"
			+ qReal::SettingsManager::value("TrikTcpServer").toString() + "\"" ;
	QString killTrikGui = "\"call killall trikGui\"";
	QString restoreLaw = "\"call chmod a+x trik/trik*\"";
	QString restartTrikGui = "\"call cd trik\" \"call ./trikGui -qws &\"";


	QString moveCommand = " \"synchronize remote trikRuntime /home/root/trik\"";

	QString command = "\"" + qReal::SettingsManager::value("WinScpPath").toString() + "\" " // path to WinScp
	"/command " + openConnection + "  "+ killTrikGui + " " + moveCommand + " " + restoreLaw + " " + restartTrikGui;

	mMainWindowInterpretersInterface->errorReporter()->addWarning(
		tr("Attention! Start downloading the runtime. Please do not turn off the robot")
	);


	myProcess.startDetached(command + " \"exit\" ");

}
