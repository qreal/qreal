#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

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
	qInstallMsgHandler(myMessageOutput);
	QCoreApplication app(argc, argv);

	if (argc != 2) {
		qDebug() << "Usage: editorPluginTestFramework fileName.qrs";
		return 1;
	}

	QString const workingCopyDir = argv[1];

	MainClass newMainClass(workingCopyDir, qApp->applicationDirPath() + "/../qrmc/", qApp->applicationDirPath());

	return 0;
}

