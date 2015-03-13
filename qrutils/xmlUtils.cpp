#include "xmlUtils.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace utils;

QDomDocument xmlUtils::loadDocument(const QString &fileName, QString *errorMessage, int *errorLine, int *errorColumn)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "cannot open file " << fileName;
		return QDomDocument();
	}

	QDomDocument doc;
	if (!doc.setContent(&file, false, errorMessage, errorLine, errorColumn))
	{
		file.close();
		return QDomDocument();
	}

	file.close();
	return doc;
}
