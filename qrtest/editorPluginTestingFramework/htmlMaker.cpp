#include "htmlMaker.h"
#include "defs.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QTranslator>
#include <QtCore/QStringList>

#include "../../qrutils/outFile.h"

using namespace editorPluginTestingFramework;
using namespace utils;

void HtmlMaker::makeHtml(QList<QPair<QString, QPair<QString, QString> > > qrxcAndQrmcResult,
			QList<QPair<QString, QPair<QString, QString> > > qrxcAndInterpreterResult)
{
	typedef QPair<QString, QPair<QString, QString> > StringTriplet;

	QDomElement root = mHtml.createElement("html");
	mHtml.appendChild(root);

	QDomElement body = mHtml.createElement("body");
	root.appendChild(body);

//	QDomText nameOfTable = mHtml.createTextNode("Table with results of comparison between qrxc and qrmc");
//	body.appendChild(nameOfTable);

//	QDomElement table = mHtml.createElement("table border");
//	body.appendChild(table);

//	addLineToTable(table, "Name of method", "Result for qrxc", "Result for qrmc", true);

//	foreach (StringTriplet const &pair, listOfLines) {
//		QString const &methodName = pair.first;
//		QString const &qrxcResult = pair.second.first;
//		QString const &qrmcResult = pair.second.second;

//		addLineToTable(table, methodName, qrxcResult, qrmcResult, false);
//	}

	addTable(body, qrxcAndQrmcResult, "Method name", "QRXC", "QRMC"); //"left", "30");
	addTable(body, qrxcAndInterpreterResult, "Method name", "QRXC", "Interpreter"); //"right", "70");

	QString const &fileName = binariesDir + "/output.html";
	OutFile outHtml(fileName);
	mHtml.save(outHtml(), 4);
}

void HtmlMaker::addTable(QDomElement parent
		, QList<QPair<QString, QPair<QString, QString> > > listOfLines
		, QString const &firstColumnTitle
		, QString const &secondColumnTitle
		, QString const &thirdColumnTitle
/*		, QString const &alignParameter
		, QString const &width*/)
{
	typedef QPair<QString, QPair<QString, QString> > StringTriplet;

	QDomElement table = mHtml.createElement("table");
	parent.appendChild(table);

	QString const &styleLine = "border:2px solid black;";
	//"float:" + alignParameter + "; width: " + width + "%;" +

	table.setAttribute("style", styleLine);
	table.setAttribute("rules", "all");

	addLineToTable(table, firstColumnTitle, secondColumnTitle, thirdColumnTitle, true);

	foreach (StringTriplet const &pair, listOfLines) {
		QString const &methodName = pair.first;
		QString const &qrxcResult = pair.second.first;
		QString const &qrmcResult = pair.second.second;

		addLineToTable(table, methodName, qrxcResult, qrmcResult, false);
	}
}

void HtmlMaker::addLineToTable(QDomElement parent
			, QString const &methodName
			, QString const &qrxcResult
			, QString const &qrmcResult
			, bool const &isTitle)
{
	QDomElement newLine = mHtml.createElement("tr");
	parent.appendChild(newLine);

	if (!isTitle) {
		QString const color = lineColor(qrxcResult, qrmcResult);
		newLine.setAttribute("bgcolor", color);
	}

	addColumnToLine(newLine, methodName);
	addColumnToLine(newLine, qrxcResult);
	addColumnToLine(newLine, qrmcResult);
}

bool HtmlMaker::resultsAreTheSame(QString const &firstMethod, QString const &secondMethod)
{
	if (firstMethod == secondMethod) {
		return true;
	}

	QSet<QString> firstMethodParsed = firstMethod.split(" ").toSet();
	QSet<QString> secondMethodParsed = secondMethod.split(" ").toSet();

	return (firstMethodParsed == secondMethodParsed);
}

QString HtmlMaker::lineColor(QString const &qrxcResult, QString const &qrmcResult)
{
	QString color = "";

	if ((containsOnly(qrmcResult, ' ')) || (qrmcResult.isEmpty())) {
		color = "Gold";
	} else {
		if (resultsAreTheSame(qrxcResult, qrmcResult)) {
			color = "PaleGreen";
		} else {
			color = "Coral";
		}
	}

	return color;
}

bool HtmlMaker::containsOnly(QString const &string, QChar const &symbol)
{
	bool containsOnlyThisSymbol = true;

	for (int i = 0; i < string.length(); i++) {
		if (string[i] != symbol) {
				containsOnlyThisSymbol = false;
		}
	}

	return containsOnlyThisSymbol;
}

void HtmlMaker::addColumnToLine(QDomElement parent, QString const &value)
{
	QDomElement newColumn = mHtml.createElement("td");
	parent.appendChild(newColumn);

	QDomText name = mHtml.createTextNode(value);
	newColumn.appendChild(name);
}
