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

// Ad kalendas graecas
#if 0
	qRealType::QRealTypeFactory::init();
#endif

	qDebug() << "Running " + args.join(" ");

	if (args.count() != 2) {
		qDebug() << "Usage: qrxc inputFile.xml";
		return 1;
	}

	QString inputXmlFileName = args[1];

	XmlCompiler xmlCompiler;
	if (!xmlCompiler.compile(inputXmlFileName))
		return 1;

	return 0;
}
