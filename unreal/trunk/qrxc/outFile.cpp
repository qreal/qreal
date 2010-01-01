#include "outFile.h"

OutFile::OutFile(QString const &fileName)
{
	mFile.setFileName(fileName);
	mFile.open(QIODevice::WriteOnly | QIODevice::Text);
	mOut.setDevice(&mFile);
}

OutFile::~OutFile()
{
	mFile.close();
}

QTextStream& OutFile::operator()()
{
	return mOut;
}