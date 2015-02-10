#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include "xmlCompiler.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
	Q_UNUSED(context)
	const char *msg = message.toLatin1().data();
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
	qInstallMessageHandler(myMessageOutput);
	QCoreApplication app(argc, argv);
	QStringList args = app.arguments();

	qDebug() << "Running " + args.join(" ");

	if (args.count() != 3) {
		qDebug() << "Usage: qrxc inputFile.xml <path to root directory of qreal sources>";
		return 1;
	}

	const QString inputXmlFileName = args[1];
	const QString root = args[2];

	XmlCompiler xmlCompiler;
	if (!xmlCompiler.compile(inputXmlFileName, root))
		return 1;

	return 0;
}
