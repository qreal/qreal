#include "outFile.h"
#include "../qrkernel/exception/exception.h"

using namespace utils;

OutFile::OutFile(QString const &fileName)
{
	mFile.setFileName(fileName);
	mFile.open(QIODevice::WriteOnly | QIODevice::Text);
	if (!mFile.isOpen()){
		throw qReal::Exception("File open operation failed");
	}
	mOut.setDevice(&mFile);
	mOut.setCodec("UTF-8");
}

QTextStream& OutFile::operator()()
{
	return mOut;
}

OutFile::~OutFile()
{
	mFile.close();
}
