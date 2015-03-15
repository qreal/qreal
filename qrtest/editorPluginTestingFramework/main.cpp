#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QTranslator>

#include "mainClass.h"

using namespace editorPluginTestingFramework;

void myMessageOutput(QtMsgType type, const char *msg)
{
	switch (type) {
		case QtDebugMsg:
			fprintf(stdout, "Debug: %s\n", msg);
			break;
		case QtWarningMsg:
			fprintf(stderr, "Warning: %s\n", msg);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "Critical: %s\n", msg);
			break;
		case QtFatalMsg:
			fprintf(stderr, "Fatal: %s\n", msg);
			abort();
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	QTranslator appTranslator;
	if (!app.arguments().contains("--no-locale")) {
		appTranslator.load(":/editorPluginTestingFramework_" + QLocale::system().name());
		app.installTranslator(&appTranslator);
	}

	if (argc != 3) {
		qDebug() << "Usage: editorPluginTestTool fileName.qrs configurationFile.xml";
		return 1;
	}
	QString const workingCopyDir = argv[1];
	QString const &configurationFileName = argv[2];
	MainClass newMainClass(workingCopyDir, qApp->applicationDirPath() + "/../../qrmc/"
			, qApp->applicationDirPath(), configurationFileName);
	return newMainClass.travisTestResult();
}

