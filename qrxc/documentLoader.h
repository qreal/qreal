#pragma once

#include <QtCore/QFileInfo>
#include <QtXml/QDomDocument>

class DocumentLoader
{
public:
	DocumentLoader();
	QDomDocument load(QFileInfo const &fileInfo);

private:
	void processElement(QDomElement &element, QDomElement &parent
		, QString const &currentDir);

	QList<QString> mUsedFiles;
};
