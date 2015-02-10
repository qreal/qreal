#include "inFile.h"
#include <qrkernel/exception/exception.h>

using namespace utils;

QString InFile::readAll(const QString &fileName)
{
	QFile file(fileName);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!file.isOpen()) {
		throw qReal::Exception((fileName + " - file open operation failed").toUtf8());
	}

	QTextStream input;
	input.setDevice(&file);
	input.setCodec("UTF-8");
	QString text = input.readAll();
	file.close();
	return text;
}
