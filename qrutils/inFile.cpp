#include "inFile.h"
#include "../qrkernel/exception/exception.h"

using namespace utils;

InFile::InFile(QString const &fileName)
{
	mFile.setFileName(fileName);
	mFile.open(QIODevice::ReadOnly| QIODevice::Text);
	if (!mFile.isOpen()){
		throw qReal::Exception("File open operation failed");
	}
	mInput.setDevice(&mFile);
	mInput.setCodec("UTF-8");
}

QTextStream& InFile::operator()()
{
	return mInput;
}

InFile::~InFile()
{
	mFile.close();
}
