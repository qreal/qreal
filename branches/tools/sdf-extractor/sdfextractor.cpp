#include "sdfextractor.h"

#include <QFile>
#include <QDebug>
#include <QSvgRenderer>
#include <QDir>

#include "pipec.h"

using namespace sdfExtractor;

SdfExtractor::SdfExtractor(QString const &fileName)
{
	if (fileName.endsWith(".xml", Qt::CaseInsensitive))
	{
		QDir dir(".");
		if (!dir.exists("svgs") )
			dir.mkdir("svgs");
		if (!dir.exists("sdfs") )
			dir.mkdir("sdfs");

		QDomDocument editor = readXml(fileName);
		QDomNodeList svgList = editor.elementsByTagName("svg_shape");
		processDomList(svgList);
		svgList = editor.elementsByTagName("svg:shape");
		processDomList(svgList);
	}
	else if (fileName.endsWith(".svg", Qt::CaseInsensitive))
	{
		QString fileNameWithoutExtension = fileName;
		fileNameWithoutExtension.chop(4);
		QDomDocument svg = readXml(fileName);
		generateSdf(svg, fileNameWithoutExtension, false);
	}
	else
	{
		qDebug() << "Unknown file extension, only .xml (for editors) and "
			<< ".svg (for svg files) are supported";
	}
}

void SdfExtractor::processDomList(QDomNodeList const &list) const
{
	for (unsigned i = 0; i < list.length(); ++i)
	{
		QDomNode svg = list.at(i);
		QString const fileName = svg.parentNode().parentNode().parentNode().toElement().attribute("id")
			+ "Class";
		generateSdf(svg, fileName, true);
	}
}

QDomDocument SdfExtractor::readXml(QString const &fileName) const
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "incorrect filename " << fileName;
		return QDomDocument();
	}

	QDomDocument document;
	QString error = "";
	int errorLine = 0;
	int errorCol = 0;
	if (!document.setContent(&file, false, &error, &errorLine, &errorCol))
	{
		file.close();
		qDebug() << "parse error in " << fileName << ", error is " << error
				<< ". error line is " << errorLine << ", column is " << errorCol;
		return QDomDocument();
	}

	file.close();
	return document;
}

void SdfExtractor::generateSdf(QDomNode const &svgNode, QString const &fileName, bool generateSvg) const
{
	int width = svgNode.toElement().attribute("width").toInt();
	int height = svgNode.toElement().attribute("height").toInt();

	if (generateSvg)
	{
		QFile file("svgs/" + fileName + ".svg");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "Error opening file " + file.fileName() + " for writing";
			return;
		}
		QTextStream stream(&file);
		svgNode.save(stream, 1);
		file.close();
	}

	QString fullFileName = fileName + ".sdf";
	if (generateSvg)
	{
		fullFileName = "sdfs/" + fullFileName;
	}

	pipec canvas(fullFileName, QPoint(width, height));
	QPainter painterf;
	QSvgRenderer rend;
	painterf.begin(&canvas);
	rend.load("svgs/" + fileName + ".svg");
	rend.render(&painterf);
}
