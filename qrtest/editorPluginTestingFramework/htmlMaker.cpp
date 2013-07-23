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

	qDebug() << "STARTING HTML GENERATING";
	QDomElement root = mHtml.createElement("html");
	mHtml.appendChild(root);

	QDomElement head = newElement(root, "head");
	QDomElement meta = newElement(head, "meta");

	QString const &charset = "utf-8";
	meta.setAttribute("charset", charset);

	QDomElement body = newElement(root, "body");

	addTable(body, qrxcAndQrmcResult, QObject::tr("Table with results of comparison between qrxc and qrmc")
			 , QObject::tr("Method name"), "QRXC", "QRMC"); //"left", "30")
	QDomElement breakLine = newElement(body, "br");
	addTable(body, qrxcAndInterpreterResult, QObject::tr("Table with results of comparison between qrxc and interpreter")
			 , QObject::tr("Method name"), "QRXC", "Interpreter"); //"right", "70");

	QString const &fileName = binariesDir + "/output.html";
	OutFile outHtml(fileName);
	mHtml.save(outHtml(), 4);
	qDebug() << "binaries/output.html generated";
}

QDomElement HtmlMaker::newElement(QDomElement &parent, QString const &newElementName)
{
	QDomElement newElement = mHtml.createElement(newElementName);
	parent.appendChild(newElement);

	return newElement;
}

void HtmlMaker::addTable(QDomElement parent
		, QList<QPair<QString, QPair<QString, QString> > > listOfLines
		, QString const &text
		, QString const &firstColumnTitle
		, QString const &secondColumnTitle
		, QString const &thirdColumnTitle)
{
	typedef QPair<QString, QPair<QString, QString> > StringTriplet;

	QDomElement bold = newElement(parent, "b");

	QDomText nameOfTable = mHtml.createTextNode(text);
	bold.appendChild(nameOfTable);

	QDomElement table = newElement(parent, "table");

	QString const &styleLine = "border:4px solid black;";

	table.setAttribute("rules", "all");
	table.setAttribute("style", styleLine);

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
	QDomElement newLine = newElement(parent, "tr");

	if (!isTitle) {
		QString const color = lineColor(qrxcResult, qrmcResult);
		newLine.setAttribute("bgcolor", color);
	}

	addColumnToLine(newLine, methodName, isTitle, true);
	addColumnToLine(newLine, qrxcResult, isTitle, false);
	addColumnToLine(newLine, qrmcResult, isTitle, false);
}

bool HtmlMaker::resultsAreTheSame(QString const &firstMethod, QString const &secondMethod)
{
	if (firstMethod == secondMethod) {
		return true;
	}

	QSet<QString> firstMethodParsed = resultToCompare(firstMethod);
	QSet<QString> secondMethodParsed = resultToCompare(secondMethod);

	return (firstMethodParsed == secondMethodParsed);
}

QString HtmlMaker::lineColor(QString const &qrxcResult, QString const &qrmcResult)
{
	QString color = "";

	if (((containsOnly(qrmcResult, ' ')) || (qrmcResult.isEmpty()))
			&& (((containsOnly(qrxcResult, ' ')) || (qrxcResult.isEmpty())))) {
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

void HtmlMaker::addColumnToLine(QDomElement parent, QString const &value, bool const &isTitle, bool const &isMethodName)
{
	QDomElement newColumn = newElement(parent, "td");

	QStringList listOfMethodTestingResults = parseOutput(value);

	foreach (QString const &elementOfOutput, listOfMethodTestingResults) {

		if (isTitle || isMethodName) {
			QDomText name = mHtml.createTextNode(elementOfOutput);
			QDomElement bold = newElement(newColumn, "b");
			bold.appendChild(name);
		} else {
			QPair<QString, QStringList> parsedOutput = parseOneElementResult(elementOfOutput);

			addTableToColumn(newColumn, parsedOutput);
		}

		QDomElement breakLine = newElement(newColumn, "br");
	}
}

void HtmlMaker::addTableToColumn(QDomElement &parent, QPair<QString, QStringList> const &tableElements)
{
	QDomElement table = newElement(parent, "table");

	QString const &align = "center";
	QString const &border = "1";
	QString const &cellspacing = "5";
	QString const &width = "100%";
	QString const &height = "100%";

	table.setAttribute("width", width);
	table.setAttribute("height", height);
	table.setAttribute("align", align);
	table.setAttribute("border", border);
	table.setAttribute("cellcpacing", cellspacing);
	table.setAttribute("rules", "all");

	addLineToResultTable(table, tableElements.first, tableElements.second);
}

void HtmlMaker::addLineToResultTable(QDomElement &parent, QString const &firstColumn, QStringList const &secondColumn)
{
	QDomElement newLine = newElement(parent, "tr");
	QString const &columnWidth = "50%";

	QDomElement firstColumnNode = newElement(newLine, "td");
	firstColumnNode.setAttribute("width", columnWidth);

	QDomText firstColumnText = mHtml.createTextNode(firstColumn);
	firstColumnNode.appendChild(firstColumnText);

	QDomElement secondColumnNode = newElement(newLine, "td");
	secondColumnNode.setAttribute("width", columnWidth);

	foreach (QString const &secondColumnElement, secondColumn) {
		QDomNode newString = mHtml.createTextNode(secondColumnElement);
		secondColumnNode.appendChild(newString);

		QDomElement breakLine = newElement(secondColumnNode, "br");
	}
}

QStringList HtmlMaker::parseOutput(QString const &methodOutput)
{
	QStringList const list = methodOutput.split("|");

	QStringList listWithoutEmptyElements;

	foreach (QString const &element, list) {
		if (!containsOnly(element, ' ')) {
			listWithoutEmptyElements.append(element);
		}
	}

	return listWithoutEmptyElements;
}

QPair<QString, QStringList> HtmlMaker::parseOneElementResult(QString const &oneElementOutput)
{
	QStringList const elementAndResult = oneElementOutput.split("-");
	QString const &element = elementAndResult.first();
	QString const &result = elementAndResult.last();

	QStringList const parsedResult = result.split(",");
	QPair<QString, QStringList> resultPair = qMakePair(element, parsedResult);

	return resultPair;
}

QSet<QString> HtmlMaker::resultToCompare(QString const &method)
{
	QStringList methodOutput = method.split("|");

	QStringList result;
	foreach (QString const &string, methodOutput) {
		QString output = string.split("-").last();
		QStringList outputToList = output.split(",");

		result.append(outputToList);
	}

	QSet<QString> methodParsed = result.toSet();
	return methodParsed;
}
