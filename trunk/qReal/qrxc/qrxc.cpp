#include <QCoreApplication>
#include <QtCore/QFile>

#include "generator.h"

// qrxc -h header_out.h -o source_out.cpp infile.xml

void usage(void)
{
	qDebug() << "qrxc -h header_out.h -o source_out.cpp infile.xml";
}

int main (int argc, char *argv[])
{
	QCoreApplication app(argc,argv);
	QStringList args = app.arguments(); // QString is better than char*
	int i = 1;
	QString header_out, source_out, infile;

	while (i < args.size())
	{
		if (args[i] == "-h")
		{
			++i;
			if (i == args.size())
			{
				qDebug() << "-h needs an argument";
				return 1;
			}
			header_out = args[i];
			++i;
			continue;
		}
		if (args[i] == "-o")
		{
			++i;
			if (i == args.size())
			{
				qDebug() << "-o needs an argument";
				return 1;
			}
			source_out = args[i];
			++i;
			continue;
		}
		if (infile == "") {
			infile = args[i];
			++i;
		} else
		{
			qDebug() << "wtf is " << infile << "?";
			return 1;
		}
	}
	if (infile == "")
	{
		qDebug() << "input XML description file missing";
		usage();
		return 1;
	}
	if (source_out == "")
	{
		qDebug() << "source out missing";
		usage();
		return 1;
	}
	if (header_out == "")
	{
		qDebug() << "header out missing";
		usage();
		return 1;
	}

	Generator g(infile, source_out, header_out);
	g.setSrcDir("editors");
//	if (!g.generate())
//		return 1;
	return 0;
}
