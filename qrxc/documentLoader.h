#pragma once

#include <QtCore/QFileInfo>
#include <QtXml/QDomDocument>

class DocumentLoader
{
public:
	DocumentLoader();
	QDomDocument load(QFileInfo const &fileInfo);

private:
	/// True means that we just processed an external reference
	void processElement(QDomElement &element, QDomElement &parent
		, QString const &currentDir);

	QList<QString> mUsedFiles;
};
