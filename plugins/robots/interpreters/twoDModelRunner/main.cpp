#include <QtCore/QDir>
#include <QtWidgets/QApplication>

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>

int const maxLogSize = 10 * 1024 * 1024;  // 10 MB

void initLogging()
{
	QDir const logsDir(QApplication::applicationDirPath() + "/logs");
	if (logsDir.mkpath(logsDir.absolutePath())) {
		qReal::Logger::addLogTarget(logsDir.filePath("2d-model.log"), maxLogSize, 2, QsLogging::DebugLevel);
	}
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	qsrand(time(0));
	initLogging();
	QLOG_INFO() << "------------------- APPLICATION STARTED --------------------";
	QLOG_INFO() << "Running on" << qReal::PlatformInfo::prettyOsVersion();
	QLOG_INFO() << "Arguments:" << app.arguments();
	QLOG_INFO() << "Setting default locale to" << QLocale().name();

	int const exitCode = app.exec();
	QLOG_INFO() << "------------------- APPLICATION FINISHED -------------------";
	return exitCode;
}
