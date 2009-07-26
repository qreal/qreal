#include <QCoreApplication>
#include <QtCore/QFile>

#include "generator.h"

// qrxc -h header_out.h -o source_out.cpp infile.xml

void usage(void)
{
	qDebug() << "Usage: ./qrxc infile.xml targetProFile.pro";
}

int main (int argc, char *argv[])
{
	QCoreApplication app(argc,argv);
	QStringList args = app.arguments(); // QString is better than char*

	if (args.count() > 1 && args[1] == "-fake_linker") {
		qDebug() << "qrxc: fake linking mode, will return 0 regardless of arguments";
		return 0;
	}

	if (args.count() != 3) {
		usage();
		return 1;
	}

	QString xmlFile = args[1];
	QString targetProFile = args[2];

	Generator g(xmlFile);
	g.generate(targetProFile);

	return 0;
}
