/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "htmlMaker.h"
#include "defs.h"
#include "convertingMethods.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QTranslator>
#include <QtCore/QStringList>

#include <qrutils/outFile.h>

using namespace editorPluginTestingFramework;
using namespace utils;

void HtmlMaker::makeHtml(
		QList<MethodsTester::ResultOfGenerating> qrxcAndQrmcResult
		, QList<MethodsTester::ResultOfGenerating> qrxcAndInterpreterResult
		, QList<MethodsTester::ResultOfGenerating> timeResult
		, QList<MethodsTester::ResultOfGenerating> timeResultIntertpter
		, const QString &pathToHtml)
{
	typedef QPair<QString, QPair<QString, QString>> StringTriplet;

	qDebug() << "STARTING HTML GENERATING";
	QDomElement root = mHtml.createElement("html");
	mHtml.appendChild(root);

	QDomElement head = newElement(root, "head");
	QDomElement meta = newElement(head, "meta");

	const QString &charset = "utf-8";
	meta.setAttribute("charset", charset);

	QDomElement body = newElement(root, "body");

	addTable(body, qrxcAndQrmcResult, QObject::tr("Table with results of comparison between qrxc and qrmc")
			 , QObject::tr("Method name"), "QRXC", "QRMC");
	QDomElement breakLine = newElement(body, "br");
	addTable(body, qrxcAndInterpreterResult, QObject::tr("Table with results of comparison between qrxc and interpreter")
			, QObject::tr("Method name"), "QRXC", "Interpreter");
	addTable(body, timeResult, QObject::tr("Table with results of time qrxc and qrmc")
			, QObject::tr("Method name"), "qrxc", "qrmc");
	addTable(body, timeResultIntertpter, QObject::tr("Table with results of time qrxc and interpreter")
			, QObject::tr("Method name"), "qrxc", "interpreter");

	const QString &fileName = pathToHtml + "/output.html";
	OutFile outHtml(fileName);
	mHtml.save(outHtml(), 4);
	qDebug() << "output.html generated";
}

QDomElement HtmlMaker::newElement(QDomElement &parent, const QString &newElementName)
{
	QDomElement newElement = mHtml.createElement(newElementName);
	parent.appendChild(newElement);

	return newElement;
}

void HtmlMaker::addTable(
		QDomElement parent
		, QList<MethodsTester::ResultOfGenerating> listOfLines
		, const QString &text
		, const QString &firstColumnTitle
		, const QString &secondColumnTitle
		, const QString &thirdColumnTitle
		)
{
	typedef QPair<QString, QPair<QString, QString> > StringTriplet;

	QDomElement bold = newElement(parent, "b");

	QDomText nameOfTable = mHtml.createTextNode(text);
	bold.appendChild(nameOfTable);

	QDomElement table = newElement(parent, "table");

	const QString &styleLine = "border:4px solid black;";

	table.setAttribute("rules", "all");
	table.setAttribute("style", styleLine);

	addLineToTable(table, firstColumnTitle, secondColumnTitle, thirdColumnTitle, true);

	for (const MethodsTester::ResultOfGenerating &element : listOfLines) {
		addLineToTable(table, element.methodName, element.firstResult, element.secondResult, false);
	}
}

void HtmlMaker::addLineToTable(QDomElement parent
			, const QString &methodName
			, const QString &firstResult
			, const QString &secondResult
			, const bool &isTitle)
{
	QDomElement newLine = newElement(parent, "tr");

	if (!isTitle) {
		const QString color = lineColor(firstResult, secondResult);
		newLine.setAttribute("bgcolor", color);
	}

	addColumnToLine(newLine, methodName, isTitle, true);
	addColumnToLine(newLine, firstResult, isTitle, false);
	addColumnToLine(newLine, secondResult, isTitle, false);
}

bool HtmlMaker::resultsAreTheSame(const QString &firstMethod, const QString &secondMethod)
{
	if (firstMethod == secondMethod) {
		return true;
	}

	const QSet<QString> firstMethodParsed = ConvertingMethods::resultToCompare(firstMethod);
	const QSet<QString> secondMethodParsed = ConvertingMethods::resultToCompare(secondMethod);

	QStringList first = firstMethodParsed.values();
	QStringList firstResult;

	QStringList second = secondMethodParsed.values();
	QStringList secondResult;

	for (const QString &element : first) {
		firstResult.append(element.toLower());
	}

	for (const QString &element : second) {
		secondResult.append(element.toLower());
	}

	for (const QString &element : firstResult) {
		bool result = secondResult.contains(element);
		if (!result) {
			return false;
		}
	}

	for (const QString &element : secondResult) {
		bool result = firstResult.contains(element);
		if (!result) {
			return false;
		}
	}

	return true;
}

QString HtmlMaker::lineColor(const QString &firstResult, const QString &secondResult)
{
	QString color = "";

	if (resultsAreEmpty(firstResult, secondResult)) {
		color = "Gold";
	} else {
		if (resultsAreTheSame(firstResult, secondResult)) {
			color = "PaleGreen";
		} else {
			if (methodFailed(firstResult, secondResult)) {
				color = "Gray";
			} else {
				color = "Coral";
			}
		}
	}

	return color;
}

bool HtmlMaker::methodFailed(const QString &firstResult, const QString &secondResult)
{
	return (firstResult.contains("method failed") || (secondResult.contains("method failed")));
}

bool HtmlMaker::containsOnly(const QString &string, QChar const &symbol)
{
	bool containsOnlyThisSymbol = true;

	for (int i = 0; i < string.length(); i++) {
		if (string[i] != symbol) {
			containsOnlyThisSymbol = false;
		}
	}

	return containsOnlyThisSymbol;
}

void HtmlMaker::addColumnToLine(QDomElement parent, const QString &value, const bool &isTitle, const bool &isMethodName)
{
	QDomElement newColumn = newElement(parent, "td");

	QStringList listOfMethodTestingResults = parseOutput(value);

	for (const QString &elementOfOutput : listOfMethodTestingResults) {
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

void HtmlMaker::addTableToColumn(QDomElement &parent, const QPair<QString, QStringList> &tableElements)
{
	QDomElement table = newElement(parent, "table");

	const QString align = "center";
	const QString border = "1";
	const QString cellspacing = "5";
	const QString width = "100%";
	const QString height = "100%";
	const QString styleLine = "border:0.2px solid black;";

	table.setAttribute("rules", "all");
	table.setAttribute("width", width);
	table.setAttribute("height", height);
	table.setAttribute("align", align);
	table.setAttribute("border", border);
	table.setAttribute("style", styleLine);
	table.setAttribute("cellcpacing", cellspacing);

	addLineToResultTable(table, tableElements.first, tableElements.second);
}

void HtmlMaker::addLineToResultTable(QDomElement &parent, const QString &firstColumn, const QStringList &secondColumn)
{
	QDomElement newLine = newElement(parent, "tr");
	const QString columnWidth = "50%";

	QDomElement firstColumnNode = newElement(newLine, "td");
	firstColumnNode.setAttribute("width", columnWidth);

	QDomText firstColumnText = mHtml.createTextNode(firstColumn);
	firstColumnNode.appendChild(firstColumnText);

	QDomElement secondColumnNode = newElement(newLine, "td");
	secondColumnNode.setAttribute("width", columnWidth);

	for (const QString &secondColumnElement : secondColumn) {
		QDomNode newString = mHtml.createTextNode(secondColumnElement);
		secondColumnNode.appendChild(newString);

		QDomElement breakLine = newElement(secondColumnNode, "br");
	}
}

QStringList HtmlMaker::parseOutput(const QString &methodOutput)
{
	const QStringList list = methodOutput.split("|");

	QStringList listWithoutEmptyElements;

	for (const QString &element : list) {
		if (!containsOnly(element, ' ')) {
			listWithoutEmptyElements.append(element);
		}
	}

	return listWithoutEmptyElements;
}

QPair<QString, QStringList> HtmlMaker::parseOneElementResult(const QString &oneElementOutput)
{
	const QStringList elementAndResult = oneElementOutput.split("-");
	const QString &element = elementAndResult.first();
	const QString &result = elementAndResult.last();

	const QStringList parsedResult = result.split(",");
	QPair<QString, QStringList> resultPair = qMakePair(element, parsedResult);

	return resultPair;
}

bool HtmlMaker::resultsAreEmpty(const QString &firstMethod, const QString &secondMethod)
{
	const QList<QString> firstMethodParsed = ConvertingMethods::resultToCompare(firstMethod).toList();
	const QList<QString> secondMethodParsed = ConvertingMethods::resultToCompare(secondMethod).toList();

	for (const QString &element : firstMethodParsed) {
		if (!containsOnly(element, ' ')) {
			return false;
		}
	}

	for (const QString &element : secondMethodParsed) {
		if (!containsOnly(element, ' ')) {
			return false;
		}
	}

	return true;
}
