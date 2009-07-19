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

	if (args.count() > 1 && args[1] == "-fake_linker") {
		qDebug() << "qrxc: fake linling mode, will return 0 regardless of arguments";
		return 0;
	}

	if (args.count() != 3) {
		usage();
		return 1;
	}

	QString targetProFile = args[2];
	QFile outFile(targetProFile);
	outFile.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&outFile);

	out << "message(\"Goodbye, world!\")" << '\n';
	out << "\n";
	out << "# TEMPLATE        =  lib\n";
	out << "CONFIG          += console\n";
	// out << "CONFIG          += plugin\n";
	out << "# DESTDIR		=  ../../../qrgui/plugins/\n";
	out << "INCLUDEPATH	+= ../..\n";
	out << "\n";
	out << "OBJECTS_DIR = ../../obj/\n";
	out << "MOC_DIR = ../../moc/\n";
	out << "\n";
	out << "include (editorsSdk.pri)\n";
	out << "SOURCES += main.cpp\n";

	outFile.close();

	{
	        QFile cppFile("generated/main.cpp");
		cppFile.open(QIODevice::WriteOnly | QIODevice::Text);
		QTextStream out(&cppFile);
	
	        out << "#include <stdio.h>\n";
	        out << "int main (int argc, char *argv[]) { printf(\"Goodbye, world!\"); }\n";
	
		cppFile.close();
	}

	return 0;
/*
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
*/
}
