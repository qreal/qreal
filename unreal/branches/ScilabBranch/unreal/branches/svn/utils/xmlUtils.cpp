#include "xmlUtils.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace utils;

QDomDocument xmlUtils::loadDocument(QString const& fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "cannot open file " << fileName;
		return QDomDocument();
	}

	QDomDocument doc;
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;

	if (!doc.setContent(&file, false, &error, &errorLine, &errorCol))
	{
		qDebug() << "parse error in " << fileName
				<< " at (" << errorLine << "," << errorCol
				<< "): " << error;
		file.close();
		return QDomDocument();
	}

	file.close();
	return doc;
}
