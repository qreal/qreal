#include "xmlCompiler.h"

#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include "typelib.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QStringList args = app.arguments();

	qRealType::QRealTypeFactory::init();

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
