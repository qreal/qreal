#include "htmlMaker.h"
#include "defs.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QTranslator>

#include "../../qrutils/outFile.h"

using namespace editorPluginTestingFramework;
using namespace utils;

void HtmlMaker::makeHtml(QList<QPair<QString, QString> > listOfLines)
{
	typedef QPair<QString, QString> StringPair;

	QDomElement root = mHtml.createElement("html");
	mHtml.appendChild(root);

	QDomElement body = mHtml.createElement("body");
	root.appendChild(body);

	QDomElement table = mHtml.createElement("table border");
	body.appendChild(table);

	foreach (StringPair const &pair, listOfLines) {
		QString const &methodName = pair.first;
		QString const &result = pair.second;

		addLineToTable(table, methodName, result);
	}

	QString const &fileName = binariesDir + "/output.html";
	OutFile outHtml(fileName);
	mHtml.save(outHtml(), 4);
}

void HtmlMaker::addLineToTable(QDomElement parent, QString const &methodName, QString const &result)
{
	QDomElement newLine = mHtml.createElement("tr");
	parent.appendChild(newLine);

	QString const color = lineColor(result);
	newLine.setAttribute("bgcolor", color);

	addColumnToLine(newLine, methodName);
	addColumnToLine(newLine, result);
}

QString HtmlMaker::lineColor(QString const &methodResult)
{
	QString color = "";

	if (methodResult == uncheckedMethod) {
		color = "Gold";
	} else {
		if (methodResult == goodMethod) {
			color = "PaleGreen";
		} else {
			color = "Coral";
		}
	}

	return color;
}

void HtmlMaker::addColumnToLine(QDomElement parent, QString const &value)
{
	QDomElement newColumn = mHtml.createElement("td");
	parent.appendChild(newColumn);

	QDomText name = mHtml.createTextNode(value);
	newColumn.appendChild(name);
}
