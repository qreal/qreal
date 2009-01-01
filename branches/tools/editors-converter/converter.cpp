#include "converter.h"

using namespace converter;

Converter::Converter(QStringList editors)
{
	foreach (QString editor, editors)
	{
		QDomDocument document = readXml("original/" + editor);
		convert(document, editor);
		writeXml(document, "converted/" + editor);
	}
}

void Converter::convert(QDomDocument &document, QString const &name)
{
	QDomNodeList shapesNodeList = document.elementsByTagName("svg_shape");
	QList<QDomNode> shapes;

	// QDomNodeList меняется при изменении документа, так что replaceChild
	// портит итератор (точнее, его жалкое подобие). Чтобы не париться, я сложил
	// нужные узлы в отдельный список.
	for (int i = 0; i < shapesNodeList.size(); ++i)
	{
		shapes.append(shapesNodeList.at(i));
	}

	foreach (QDomNode shape, shapes)
	{
		QDomNode node = shape.parentNode().parentNode().parentNode();
		QString shapeName = node.toElement().attribute("id");
		QString shapeFileName = "sdf/" + shapeName + "Class.sdf";

		QDomDocument sdfShapeDocument = readXml(shapeFileName);
		if (sdfShapeDocument == QDomDocument())
		{
			qDebug() << "Achtung! " + shapeFileName + " not found, no graphics generated!";
			continue;
		}

		Q_ASSERT(sdfShapeDocument.elementsByTagName("picture").count() == 1);
		QDomNode sdfNode = sdfShapeDocument.elementsByTagName("picture").at(0);
		shape.parentNode().replaceChild(sdfNode, shape);
	}
}

QDomDocument Converter::readXml(QString const &fileName)
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

void Converter::writeXml(QDomDocument const &document, QString const &fileName)
{
	QFile file(fileName);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
	{
		qDebug() << "incorrect filename " << fileName;
		return;
	}

	QTextStream stream;
	stream.setDevice(&file);

	stream << document.toString();

	file.close();
}
