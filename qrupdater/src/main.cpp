#include <QtCore/QCoreApplication>

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>

#include "updateProcessor.h"

int const maxLogSize = 10 * 1024 * 1024;  // 10 MB

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	QDir const logsDir(app.applicationDirPath() + "/logs");
	if (logsDir.mkpath(logsDir.absolutePath())) {
		qReal::Logger::addLogTarget(logsDir.filePath("updater.log"), maxLogSize, 2, QsLogging::DebugLevel);
	}

	QLOG_INFO() << "------------------- UPDATER STARTED -------------------";
	QLOG_INFO() << "Running on" << qReal::PlatformInfo::printableInfo();
	QLOG_INFO() << "Arguments:" << app.arguments();

	qrUpdater::UpdateProcessor updater;
	updater.startUpdateControl();

	int const exitCode = app.exec();
	QLOG_INFO() << "------------------- UPDATER FINISHED -------------------";
	return exitCode;
}
