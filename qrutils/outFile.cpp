#include "outFile.h"

#include <qrkernel/logging.h>
#include <qrkernel/exception/exception.h>

using namespace utils;

OutFile::OutFile(const QString &fileName)
{
	mFile.setFileName(fileName);
	mFile.open(QIODevice::WriteOnly | QIODevice::Text);
	if (!mFile.isOpen()) {
		throw qReal::Exception("File open operation failed");
	}

	mOut.setDevice(&mFile);
	mOut.setCodec("UTF-8");
}

QTextStream &OutFile::operator()()
{
	return mOut;
}

OutFile *OutFile::openOrLogError(const QString &fileName)
{
	if (fileName.isEmpty()) {
		return nullptr;
	}

	try {
		utils::OutFile * const file = new utils::OutFile(fileName);
		return file;
	} catch (const qReal::Exception &exception) {
		QLOG_ERROR() << exception.message();
	}

	return nullptr;
}

OutFile::~OutFile()
{
	mFile.close();
}
