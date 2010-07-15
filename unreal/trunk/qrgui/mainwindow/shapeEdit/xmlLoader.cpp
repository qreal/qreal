#include "xmlLoader.h"
#include "../../../utils/xmlUtils.h"

#include <QtCore/QDebug>

XmlLoader::XmlLoader(Scene *scene)
{
	mScene = scene;
}

void XmlLoader::readString(const QString &text)
{
	mDocument.setContent(text);
	QDomNodeList const graphics = mDocument.elementsByTagName("graphics");

	for (unsigned i = 0; i < graphics.length(); ++i) {
		QDomElement graphic = graphics.at(i).toElement();
		readGraphics(graphic);
	}
}

void XmlLoader::readFile(const QString &fileName)
{
	mDocument = utils::xmlUtils::loadDocument(fileName);
	QDomNodeList const graphics = mDocument.elementsByTagName("graphics");

	for (unsigned i = 0; i < graphics.length(); ++i) {
		QDomElement graphic = graphics.at(i).toElement();
		readGraphics(graphic);
	}
}

void XmlLoader::readGraphics(QDomElement const &graphic)
{
	QDomNodeList graphicAttributes = graphic.childNodes();

	for (unsigned i = 0; i < graphicAttributes.length(); ++i) {
		QDomElement type = graphicAttributes.at(i).toElement();
		if (type.tagName() == "picture")
			readPicture(type);
		else if (type.tagName() == "labels")
			readLabels(type);
		else if (type.tagName() == "ports")
			readPorts(type);
		else
			qDebug() << "ololo_graphics";
	}
}

void XmlLoader::readPicture(QDomElement const &picture)
{
	QDomNodeList pictureAttributes = picture.childNodes();

	qreal sizePictureX = (picture.attribute("sizex", "")).toInt();
	qreal sizePictureY = (picture.attribute("sizey", "")).toInt();
	mDrift = QPointF(mScene->centerEmpty().x() - sizePictureX / 2, mScene->centerEmpty().y() - sizePictureY / 2);

	for (unsigned i = 0; i < pictureAttributes.length(); ++i) {
		QDomElement type = pictureAttributes.at(i).toElement();
		if (type.tagName() == "line")
			readLine(type);
		else if (type.tagName() == "ellipse")
			readEllipse(type);
		else if (type.tagName() == "arc")
			readArch(type);
		else if (type.tagName() == "rectangle")
			readRectangle(type);
		else if (type.tagName() == "stylus")
			readStylus(type);
		else if (type.tagName() == "path")
			readPath(type);
		else
			qDebug() << "ololo_picture";
	}
}

void XmlLoader::readLabels(QDomElement const &label)
{
	QDomNodeList labelAttributes = label.childNodes();

	for (unsigned i = 0; i < labelAttributes.length(); ++i) {
		QDomElement type = labelAttributes.at(i).toElement();
		if (type.tagName() == "label")
			readLabel(type);
		else
			qDebug() << "ololo_labelS";
	}
}

void XmlLoader::readPorts(QDomElement const &port)
{
	QDomNodeList portAttributes = port.childNodes();

	for (unsigned i = 0; i < portAttributes.length(); ++i) {
		QDomElement type = portAttributes.at(i).toElement();
		if (type.tagName() == "linePort")
			readLinePort(type);
		else if (type.tagName() == "pointPort")
			readPointPort(type);
		else
			qDebug() << "ololo_ports";
	}
}

QRectF XmlLoader::readRectOfXandY(QDomElement const &docItem)
{
	qreal x1 = (docItem.attribute("x1", "0")).toDouble() + mDrift.x();
	qreal x2 = (docItem.attribute("x2", "0")).toDouble() + mDrift.x();
	qreal y1 = (docItem.attribute("y1", "0")).toDouble() + mDrift.y();
	qreal y2 = (docItem.attribute("y2", "0")).toDouble() + mDrift.y();
	return QRectF(x1, y1, x2 - x1 + 1, y2 - y1 + 1);
}

QPointF XmlLoader::readXandY(QDomElement const &docItem)
{
	qreal x = (docItem.attribute("x", "0")).toDouble() + mDrift.x();
	qreal y= (docItem.attribute("y", "0")).toDouble() + mDrift.y();
	return QPointF(x, y);
}

void XmlLoader::readLine(QDomElement const &line)
{
	QRectF rect = readRectOfXandY(line);
	Line* item = new Line(rect.left(), rect.top(), rect.right(), rect.bottom(), NULL);
	item->readPenBrush(line);
	mScene->addItem(item);
}

void XmlLoader::readEllipse(QDomElement const &ellipse)
{
	QRectF rect = readRectOfXandY(ellipse);
	Ellipse* item = new Ellipse(rect.left(), rect.top(), rect.right(), rect.bottom(), NULL);
	item->readPenBrush(ellipse);
	mScene->addItem(item);
}

void XmlLoader::readArch(QDomElement const &arch)
{
	QRectF rect = readRectOfXandY(arch);
	Arch* item = new Arch(rect.left(), rect.top(), rect.right(), rect.bottom(), NULL);
	int spanAngle = arch.attribute("spanAngle", "0").toInt();
	int startAngle = arch.attribute("startAngle", "0").toInt();
	item->setSpanAngle(spanAngle);
	item->setStartAngle(startAngle);
	mScene->addItem(item);
}

void XmlLoader::readRectangle(QDomElement const &rectangle)
{
	QRectF rect = readRectOfXandY(rectangle);
	Rectangle* item = new Rectangle(rect.left(), rect.top(), rect.right(), rect.bottom(), NULL);
	item->readPenBrush(rectangle);
	mScene->addItem(item);
}

void XmlLoader::readStylus(QDomElement const &stylus)
{
	QDomNodeList stylusAttributes = stylus.childNodes();

	Stylus* stylusItem = new Stylus(0, 0, NULL);
	for (unsigned i = 0; i < stylusAttributes.length(); ++i) {
		QDomElement type = stylusAttributes.at(i).toElement();
		if (type.tagName() == "line") {
			QRectF rect = readRectOfXandY(type);
			Line* item = new Line(rect.left(), rect.top(), rect.right(), rect.bottom(), NULL);
			item->readPenBrush(type);
			stylusItem->mListLine.push_back(item);
		}
		else
			qDebug() << "ololo_stylus";
	}
	mScene->addItem(stylusItem);
}

bool XmlLoader::isNotLCMZ(QString str, int i)
{
	return (str[i] != 'L') && (str[i] != 'C') && (str[i] != 'M')
		&& (str[i] != 'Z') && (i != str.length());
}

void XmlLoader::readPath(QDomElement const &element)
{
	QPointF end_point;
	QPointF c1;
	QPointF c2;
	QDomElement elem = element;
	QPainterPath path;

	if (!elem.isNull())
	{
		QString d_cont;
		d_cont = elem.attribute("d").remove(0, 1);
		d_cont.append(" Z");

		for (i = 0; i < d_cont.length() - 1;)
		{
			if (d_cont[i] == 'M')
			{
				j = i + 2;
				while (isNotLCMZ(d_cont, j))
				{
					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					end_point.setX(s1.toFloat()+ mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					end_point.setY(s1.toFloat() + mStartY);
					++j;
					s1.clear();
				}

				path.moveTo(end_point);
				i = j;
			}
			else if (d_cont[i] == 'L')
			{
				j = i + 2;
				while (isNotLCMZ(d_cont, j))
				{
					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					end_point.setX(s1.toFloat()+ mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}
					end_point.setY(s1.toFloat() + mStartY);
					++j;
					s1.clear();
				}

				 path.lineTo(end_point);
				 i = j;
			}
			 else if (d_cont[i] == 'C')
			{
				j = i + 2;
				while(isNotLCMZ(d_cont, j))
				{
					while (!(d_cont[j] == ' '))
					{
						s1.append(d_cont[j]);
						++j;
					}

					c1.setX(s1.toFloat() + mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					c1.setY(s1.toFloat() + mStartY);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					c2.setX(s1.toFloat() + mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					c2.setY(s1.toFloat() + mStartY);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					end_point.setX(s1.toFloat() + mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					end_point.setY(s1.toFloat() + mStartY);
					s1.clear();
					++j;
				}

				path.cubicTo(c1, c2, end_point);
				i = j;

			} else if (d_cont[i] == 'Z')
			{
				path.closeSubpath();
			}
		}
	}
	Path *item = new Path(path);
	item->translate(mDrift.x(), mDrift.y());
	item->readPenBrush(elem);
	mScene->addItem(item);
}

void XmlLoader::readLabel(QDomElement const &label)
{
	QPointF point = readXandY(label);
	Text* item = NULL;
	if (label.hasAttribute("text")) {
		item = new Text(point.x(), point.y(), label.attribute("text", ""));
		item->setIsDynamicText(false);
	} else if (label.hasAttribute("textBinded")) {
		item = new Text(point.x(), point.y(), label.attribute("textBinded", ""));
		item->setIsDynamicText(true);
	}
	else
		qDebug() << "ololo_label";
	mScene->addItem(item);
}

void XmlLoader::readLinePort(QDomElement const &linePort)
{
	QDomNodeList linePortAttributes = linePort.childNodes();
	qreal x1 = 0;
	qreal y1 = 0;
	qreal x2 = 0;
	qreal y2 = 0;
	for (unsigned i = 0; i < linePortAttributes.length(); ++i) {
		QDomElement type = linePortAttributes.at(i).toElement();
		if (type.tagName() == "start") {
			x1 = (type.attribute("startx", "0")).toDouble() + mDrift.x();
			y1 = (type.attribute("starty", "0")).toDouble() + mDrift.y();
		}
		else if (type.tagName() == "end") {
			x2 = (type.attribute("endx", "0")).toDouble() + mDrift.x();
			y2 = (type.attribute("endy", "0")).toDouble() + mDrift.y();
		} else
			qDebug() << "ololo_linePort";

	}
	LinePort* item = new LinePort(x1, y1, x2, y2, NULL);
	mScene->addItem(item);
}

void XmlLoader::readPointPort(QDomElement const &pointPort)
{
	QPointF point = readXandY(pointPort);
	PointPort* item = new PointPort(point.x(), point.y(), NULL);
	mScene->addItem(item);
}
