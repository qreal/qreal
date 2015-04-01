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
		"Passed .qrs will be interpreted just like when 'Run' button was pressed in TRIK Studio. \n"\
		"In background mode the session will be terminated just after the execution ended and return code "
		"will then contain binary information about program correctness."
		"Example: \n") +
		"    2D-model -b --platform minimal --report report.json --trajectory trajectory.fifo example.qrs";

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
	QCommandLineOption platformOption("platform"
			, QObject::tr("Use this option set to \"minimal\" to disable connection to X server"), "minimal");
	QCommandLineOption reportOption("report", QObject::tr("A path to file where checker results will be written (JSON)")
			, "path-to-report", "report.json");
	QCommandLineOption trajectoryOption("trajectory", QObject::tr("A path to file where robot`s trajectory will be"\
				" written. The writing will not be performed not immediately, each trajectory point will be written"\
				" just when obtained by checker, engine so FIFOs are recommended to be targets for this option.")
			, "path-to-trajectory", "trajectory.fifo");
	parser.addOption(backgroundOption);
	parser.addOption(platformOption);
	parser.addOption(reportOption);
	parser.addOption(trajectoryOption);

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
	const QString report = parser.isSet(reportOption) ? parser.value(reportOption) : QString();
	const QString trajectory = parser.isSet(trajectoryOption) ? parser.value(trajectoryOption) : QString();
	twoDModel::Runner runner(report, trajectory);
	runner.interpret(qrsFile, backgroundMode);

	const int exitCode = app.exec();
	QLOG_INFO() << "------------------- APPLICATION FINISHED -------------------";
	return exitCode;
}
