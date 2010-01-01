#pragma once

#include <QFile>
#include <QTextStream>

class OutFile
{
public:
	explicit OutFile(QString const &fileName);
	~OutFile();
	QTextStream& operator()();

private:
	QTextStream mOut;
	QFile mFile;
};


