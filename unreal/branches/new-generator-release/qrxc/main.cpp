#include "xmlCompiler.h"

#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc,argv);
	QStringList args = app.arguments();

	if (args.count() != 2) {
		qDebug() << "Usage: qrxc inputFile.xml";
		return 1;
	}

	QString inputXmlFileName = args[1];

	XmlCompiler xmlCompiler;
	xmlCompiler.compile(inputXmlFileName);

	return 0;
}
