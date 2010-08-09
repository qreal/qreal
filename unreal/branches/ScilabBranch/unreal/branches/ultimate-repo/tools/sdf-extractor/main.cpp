#include <QtCore/QCoreApplication>
#include <QDebug>

#include "sdfextractor.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	if (argc != 2)
	{
		qDebug() << "Usage: sdfextractor <.xml editor specification file name>";
		return 1;
	}

	QString fileName = argv[1];
	sdfExtractor::SdfExtractor extractor(fileName);
	return 0;
}
