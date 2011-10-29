#include "outFile.h"


#include "../qrkernel/exception/exception.h"

using namespace utils;

OutFile::OutFile(QString const &fileName)
	: mIndent(0)
{
	mFile.setFileName(fileName);
	mFile.open(QIODevice::WriteOnly | QIODevice::Text);
	if (!mFile.isOpen())
		throw qReal::Exception("File open operation failed");
	mOut.setDevice(&mFile);
	mOut.setCodec("UTF-8");
}

OutFile::~OutFile()
{
	mFile.close();
}

QTextStream& OutFile::operator()()
{
	mOut << indent();
	return mOut;
}

void OutFile::incIndent()
{
	++mIndent;
}

void OutFile::decIndent()
{
	--mIndent;
}

QString OutFile::indent() const
{
	QString result;
	for (int i = 0; i < mIndent; ++i)
		result += "\t";
	return result;
}
