#include "xmlCompiler.h"

#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

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
	QStringList args = app.arguments();

	qDebug() << "Running " + args.join(" ");

	if (args.count() != 3) {
		qDebug() << "Usage: qrxc inputFile.xml <path to root directory of qreal sources>";
		return 1;
	}

	QString const inputXmlFileName = args[1];
	QString const root = args[2];

	XmlCompiler xmlCompiler;
	if (!xmlCompiler.compile(inputXmlFileName, root))
		return 1;

	return 0;
}
