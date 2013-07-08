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
			, QString const &text
			, QString const &firstColumnTitle
			, QString const &secondColumnTitle
			, QString const &thirdColumnTitle);

	void addLineToTable(QDomElement parent
			, QString const &methodName
			, QString const &qrxcResult
			, QString const &qrmcResult
			, bool const &isTitle);

	QDomElement newElement(QDomElement &parent, QString const &newElementName);

	void addColumnToLine(QDomElement parent, QString const &value, bool const &isTitle, bool const &isMethodName);

	static bool containsOnly(QString const &string, QChar const &symbol);
	static bool resultsAreTheSame(QString const &firstMethod, QString const &secondMethod);

	static QStringList parseOutput(QString const &methodOutput);
	static QString lineColor(QString const &qrxcResult, QString const &qrmcResult);

	QDomDocument mHtml;
};

}

