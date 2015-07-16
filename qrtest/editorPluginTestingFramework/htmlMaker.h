#pragma once

#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QPair>
#include "methodsTester.h"

namespace editorPluginTestingFramework {

class HtmlMaker
{
public:
	/// creates .html-file with results of testing
	void makeHtml(QList<MethodsTester::ResultOfGenerating> qrxcAndQrmcResult
			, QList<MethodsTester::ResultOfGenerating> qrxcAndInterpreterResult
			, QList<MethodsTester::ResultOfGenerating> timeResult
			, QList<MethodsTester::ResultOfGenerating> timeResultIntertpter
			, const QString &pathToHtml);

private:
	/// adds one table with results of testing for qrxc-qrmc or qrxc-interpreter
	void addTable(QDomElement parent
			, QList<MethodsTester::ResultOfGenerating> listOfLines
			, const QString &text
			, const QString &firstColumnTitle
			, const QString &secondColumnTitle
			, const QString &thirdColumnTitle
			);

	/// adds one line to table
	void addLineToTable(
			QDomElement parent
			, const QString &methodName
			, const QString &firstResult
			, const QString &secondResult
			, const bool &isTitle
			);

	/// creates new qDomElement
	QDomElement newElement(QDomElement &parent, const QString &newElementName);

	/// adds column to line
	void addColumnToLine(
			QDomElement parent
			, const QString &value
			, const bool &isTitle
			, const bool &isMethodName);

	/// adds inner table into column (table for results of testing one method of one interface)
	void addTableToColumn(
			QDomElement &parent
			, const QPair<QString, QStringList> &tableElements
			);

	/// adds line to inner table
	void addLineToResultTable(
			QDomElement &parent
			, const QString &firstColumn
			, const QStringList &secondColumn
			);

	/// finds out if string consists only of given symbol (for example, "aaa" consists only of symbol 'a')
	static bool containsOnly(const QString &string, const QChar &symbol);

	/// finds out if results of testing are equivalent
	static bool resultsAreTheSame(const QString &firstMethod, const QString &secondMethod);

	/// checks if results are empty
	static bool resultsAreEmpty(const QString &firstMethod, const QString &secondMethod);

	/// checks if method failed
	static bool methodFailed(const QString &firstResult, const QString &secondResult);

	/// parses results of methods testing
	static QStringList parseOutput(const QString &methodOutput);

	/// parses given result of testing for further adding to table
	static QPair<QString, QStringList> parseOneElementResult(const QString &oneElementOutput);

	/// returns color of line (red, green or red, depending on methods result concurrence)
	static QString lineColor(const QString &firstResult, const QString &secondResult);

	QDomDocument mHtml;
};

}

