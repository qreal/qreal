#include <time.h>

#include <QtCore/QDir>
#include <QtCore/QCommandLineParser>
#include <QtWidgets/QApplication>

#include <qrkernel/logging.h>
#include <qrkernel/platformInfo.h>

#include "runner.h"

const int maxLogSize = 10 * 1024 * 1024;  // 10 MB

const QString description = QObject::tr(
		"Emulates robot`s behaviour on TRIK Studio 2D model separately from programming environment. "\
		"Passed .qrs will be interpreted just like when 'Run' button was pressed in TRIK Studio."
);

void initLogging()
{
	const QDir logsDir(QApplication::applicationDirPath() + "/logs");
	if (logsDir.mkpath(logsDir.absolutePath())) {
		qReal::Logger::addLogTarget(logsDir.filePath("2d-model.log"), maxLogSize, 2, QsLogging::DebugLevel);
	}
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName("2D-model");
	QCoreApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription(description);
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("qrs-file", QObject::tr("Save file to be interpreted."));
	QCommandLineOption backgroundOption({"b", "background"}, QObject::tr("Run emulation in background."));
	parser.addOption(backgroundOption);

	qsrand(time(0));
	initLogging();
	QLOG_INFO() << "------------------- APPLICATION STARTED --------------------";
	QLOG_INFO() << "Running on" << qReal::PlatformInfo::prettyOsVersion();
	QLOG_INFO() << "Arguments:" << app.arguments();
	QLOG_INFO() << "Setting default locale to" << QLocale().name();

	parser.process(app);

	const QStringList positionalArgs = parser.positionalArguments();
	if (positionalArgs.size() != 1) {
		parser.showHelp();
	}

	const QString qrsFile = positionalArgs.first();
	const bool backgroundMode = parser.isSet(backgroundOption);
	twoDModel::Runner runner;
	runner.interpret(qrsFile, backgroundMode);

	const int exitCode = app.exec();
	QLOG_INFO() << "------------------- APPLICATION FINISHED -------------------";
	return exitCode;
}
