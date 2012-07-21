#include "inFile.h"
#include "../qrkernel/exception/exception.h"

using namespace utils;

InFile::InFile()
{
}

QString InFile::readAll(QString const &fileName) {
	QFile file(fileName);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!file.isOpen()) {
		throw qReal::Exception("File open operation failed");
	}
	QTextStream input;
	input.setDevice(&file);
	input.setCodec("UTF-8");
	QString text = input.readAll();
	file.close();
	return text;
}

InFile::~InFile()
{
}
