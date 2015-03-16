#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QPair>

namespace editorPluginTestingFramework {

class HtmlMaker
{
public:
	/// creates .html-file with results of testing
	void makeHtml(QList<QPair<QString, QPair<QString, QString> > > qrxcAndQrmcResult
			, QList<QPair<QString, QPair<QString, QString> > > qrxcAndInterpreterResult
			, QList<QPair<QString, QPair<QString, QString> > > timeResult, QList<QPair<QString, QPair<QString, QString> > > timeResultIntertpter, QString const &pathToHtml);

private:
	/// adds one table with results of testing for qrxc-qrmc or qrxc-interpreter
	void addTable(
			QDomElement parent
			, QList<QPair<QString, QPair<QString, QString> > > listOfLines
			, QString const &text
			, QString const &firstColumnTitle
			, QString const &secondColumnTitle
			, QString const &thirdColumnTitle
			);

	/// adds one line to table
	void addLineToTable(
			QDomElement parent
			, QString const &methodName
			, QString const &firstResult
			, QString const &secondResult
			, bool const &isTitle
			);

	/// creates new qDomElement
	QDomElement newElement(QDomElement &parent, QString const &newElementName);

	/// adds column to line
	void addColumnToLine(
			QDomElement parent
			, QString const &value
			, bool const &isTitle
			, bool const &isMethodName);

	/// adds inner table into column (table for results of testing one method of one interface)
	void addTableToColumn(
			QDomElement &parent
			, QPair<QString, QStringList> const &tableElements
			);

	/// adds line to inner table
	void addLineToResultTable(
			QDomElement &parent
			, QString const &firstColumn
			, QStringList const &secondColumn
			);

	/// finds out if string consists only of given symbol (for example, "aaa" consists only of symbol 'a')
	static bool containsOnly(QString const &string, QChar const &symbol);
	/// finds out if results of testing are equivalent
	static bool resultsAreTheSame(QString const &firstMethod, QString const &secondMethod);
	/// checks if results are empty
	static bool resultsAreEmpty(QString const &firstMethod, QString const &secondMethod);

	/// checks if method failed
	static bool methodFailed(QString const &firstResult, QString const &secondResult);

	/// parses results of methods testing
	static QStringList parseOutput(QString const &methodOutput);

	/// parses given result of testing for further adding to table
	static QPair<QString, QStringList> parseOneElementResult(QString const &oneElementOutput);

	/// returns color of line (red, green or red, depending on methods result concurrence)
	static QString lineColor(QString const &firstResult, QString const &secondResult);

	QDomDocument mHtml;
};

}

