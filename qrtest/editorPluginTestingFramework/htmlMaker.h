#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QPair>

namespace editorPluginTestingFramework {

class HtmlMaker
{
public:
	void makeHtml(QList<QPair<QString, QString> > listOfLines);

private:
	void addLineToTable(QDomElement parent, QString const &methodName, QString const &result);
	void addColumnToLine(QDomElement parent, QString const &value);

	static QString lineColor(QString const &methodResult);

	QDomDocument mHtml;
};

}

