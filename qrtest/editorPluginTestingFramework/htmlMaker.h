#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QPair>

namespace editorPluginTestingFramework {

class HtmlMaker
{
public:
	void makeHtml(QList<QPair<QString, QPair<QString, QString> > > qrxcAndQrmcResult,
			QList<QPair<QString, QPair<QString, QString> > > qrxcAndInterpreterResult);

private:
	void addTable(QDomElement parent
			, QList<QPair<QString, QPair<QString, QString> > > listOfLines
			, QString const &firstColumnTitle
			, QString const &secondColumnTitle
			, QString const &thirdColumnTitle
/*			, QString const &alignParameter
			, QString const &width*/);

	void addLineToTable(QDomElement parent
			, QString const &methodName
			, QString const &qrxcResult
			, QString const &qrmcResult
			, bool const &isTitle);

	void addColumnToLine(QDomElement parent, QString const &value);
	static bool containsOnly(QString const &string, QChar const &symbol);
	static bool resultsAreTheSame(QString const &firstMethod, QString const &secondMethod);

	static QString lineColor(QString const &qrxcResult, QString const &qrmcResult);

	QDomDocument mHtml;
};

}

