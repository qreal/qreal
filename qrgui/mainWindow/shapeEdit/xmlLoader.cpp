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

#include "xmlLoader.h"

#include <QtCore/QDir>
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>

#include <qrutils/xmlUtils.h>

using namespace qReal;

XmlLoader::XmlLoader(Scene *scene)
{
	mScene = scene;
	initListScalePoint();
	mStrX = 0;
	mStrY = 0;
	mFloorY = 0;
	mReadFile = true;
}

void XmlLoader::initListScalePoint()
{
	mListScalePoint.clear();
	mListScalePoint.push_back(QPair<Item::ScalingPointState, QColor>(Item::topLeftX, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<Item::ScalingPointState, QColor>(Item::topRightX, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<Item::ScalingPointState, QColor>(Item::bottomLeftX, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<Item::ScalingPointState, QColor>(Item::bottomRightX, QColor(Qt::black)));

	mListScalePoint.push_back(QPair<Item::ScalingPointState, QColor>(Item::topLeftY, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<Item::ScalingPointState, QColor>(Item::topRightY, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<Item::ScalingPointState, QColor>(Item::bottomLeftY, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<Item::ScalingPointState, QColor>(Item::bottomRightY, QColor(Qt::black)));
}

void XmlLoader::readString(const QString &text)
{
	mReadFile = false;
	mDocument.setContent(text);
	readDocument();
}

void XmlLoader::readFile(const QString &fileName)
{
	mReadFile = true;
	mDocument = utils::xmlUtils::loadDocument(fileName);
	readDocument();
}

void XmlLoader::readDocument()
{
	const QDomNodeList graphics = mDocument.elementsByTagName("graphics");
	for (int i = 0; i < graphics.length(); ++i) {
		QDomElement graphic = graphics.at(i).toElement();
		readGraphics(graphic);
	}
}

void XmlLoader::readGraphics(const QDomElement &graphic)
{
	QDomNodeList graphicAttributes = graphic.childNodes();

	int sizePictureX = 0;
	int sizePictureY = 0;

	for (int i = 0; i < graphicAttributes.length(); ++i) {
		QDomElement type = graphicAttributes.at(i).toElement();
		if (type.tagName() == "picture") {
			sizePictureX = (type.attribute("sizex", "")).toInt();
			sizePictureY = (type.attribute("sizey", "")).toInt();
			if (mReadFile) {
				if (mStrX + distanceFigure + sizePictureX >= sizeEmptyRectX) {
					mStrY = mFloorY;
					mStrX = 0;
				}
				mDrift = QPoint(mStrX + distanceFigure, mStrY + distanceFigure);
			} else
				mDrift = QPoint(mScene->centerEmpty().x() - sizePictureX / 2
						, mScene->centerEmpty().y() - sizePictureY / 2);
			readPicture(type);
		}
		else if (type.tagName() == "labels")
			readLabels(type);
		else if (type.tagName() == "ports")
			readPorts(type);
		else
			qDebug() << "Incorrect graphics tag";
	}

	if (mReadFile) {
		mFloorY = qMax(mFloorY, mDrift.y() + sizePictureY);
		mStrX = mDrift.x() + sizePictureX;
	}
}

void XmlLoader::readPicture(const QDomElement &picture)
{
	QDomNodeList pictureAttributes = picture.childNodes();

	for (int i = 0; i < pictureAttributes.length(); ++i) {
		QDomElement type = pictureAttributes.at(i).toElement();
		if (type.tagName() == "line") {
			readLine(type);
		} else if (type.tagName() == "ellipse") {
			readEllipse(type);
		} else if (type.tagName() == "arc") {
			readArch(type);
		} else if (type.tagName() == "rectangle") {
			readRectangle(type);
		} else if (type.tagName() == "stylus") {
			readStylus(type);
		} else if (type.tagName() == "path") {
			readPath(type);
		} else if (type.tagName() == "curve") {
			readCurve(type);
		} else if (type.tagName() == "text") {
			readText(type);
		} else if (type.tagName() == "image") {
			readImage(type);
		} else {
			qDebug() << "Incorrect picture tag";
		}
	}
}

void XmlLoader::readLabels(const QDomElement &label)
{
	QDomNodeList labelAttributes = label.childNodes();

	for (int i = 0; i < labelAttributes.length(); ++i) {
		QDomElement type = labelAttributes.at(i).toElement();
		if (type.tagName() == "label") {
			readLabel(type);
		} else {
			qDebug() << "Incorrect labels tag";
		}
	}
}

void XmlLoader::readPorts(const QDomElement &port)
{
	QDomNodeList portAttributes = port.childNodes();

	for (int i = 0; i < portAttributes.length(); ++i) {
		QDomElement type = portAttributes.at(i).toElement();
		if (type.tagName() == "linePort") {
			readLinePort(type);
		} else if (type.tagName() == "pointPort")
			readPointPort(type);
		else {
			qDebug() << "Incorrect ports tag";
		}
	}
}

QPair<QString, bool> XmlLoader::readScaleCoord(QString point, const QDomElement &docItem)
{
	QString text = docItem.attribute(point, "0");
	if (text.endsWith("a")) {
		text.truncate(text.size() - 1);
		return QPair<QString, bool>(text, true);
	}
	return QPair<QString, bool>(text, false);
}

void XmlLoader::changeScaleColor(int i)
{
	mListScalePoint.insert(i, QPair<Item::ScalingPointState, QColor>(mListScalePoint.at(i).first, QColor(Qt::red)));
	mListScalePoint.removeAt(i + 1);
}

void XmlLoader::checkScale(QPair<QString, bool> pointX1, QPair<QString, bool> pointX2
		, QPair<QString, bool> pointY1, QPair<QString, bool> pointY2)
{
	if (pointX1.second) {
		changeScaleColor(0);
	}

	if (pointX2.second) {
		changeScaleColor(3);
	}

	if (pointY1.second) {
		changeScaleColor(4);
	}

	if (pointY2.second) {
		changeScaleColor(7);
	}
}

QRectF XmlLoader::readRectOfXandY(const QDomElement &docItem)
{
	initListScalePoint();
	QPair<QString, bool> pointX1 = readScaleCoord("x1", docItem);
	QPair<QString, bool> pointX2 = readScaleCoord("x2", docItem);
	QPair<QString, bool> pointY1 = readScaleCoord("y1", docItem);
	QPair<QString, bool> pointY2 = readScaleCoord("y2", docItem);

	qreal x1 = pointX1.first.toDouble() + mDrift.x();
	qreal x2 = pointX2.first.toDouble() + mDrift.x();
	qreal y1 = pointY1.first.toDouble() + mDrift.y();
	qreal y2 = pointY2.first.toDouble() + mDrift.y();

	checkScale(pointX1, pointX2, pointY1, pointY2);

	return QRectF(x1, y1, x2 - x1, y2 - y1);
}

QPair<QPointF, QPointF> XmlLoader::calcLineOfXandY(QPair<QString, bool> pointX1, QPair<QString, bool> pointX2
		, QPair<QString, bool> pointY1, QPair<QString, bool> pointY2)
{
	qreal x1 = pointX1.first.toDouble() + mDrift.x();
	qreal x2 = pointX2.first.toDouble() + mDrift.x();
	qreal y1 = pointY1.first.toDouble() + mDrift.y();
	qreal y2 = pointY2.first.toDouble() + mDrift.y();

	if (x2 > x1) {
		if (y2 > y1) {
			if (pointX1.second)
				changeScaleColor(0);
			if (pointX2.second)
				changeScaleColor(3);
			if (pointY1.second)
				changeScaleColor(4);
			if (pointY2.second)
				changeScaleColor(7);
		} else {
			if (pointX1.second)
				changeScaleColor(2);
			if (pointX2.second)
				changeScaleColor(1);
			if (pointY1.second)
				changeScaleColor(6);
			if (pointY2.second)
				changeScaleColor(5);
		}
	} else {
		if (y2 > y1) {
			if (pointX1.second)
				changeScaleColor(1);
			if (pointX2.second)
				changeScaleColor(2);
			if (pointY1.second)
				changeScaleColor(5);
			if (pointY2.second)
				changeScaleColor(6);
		} else {
			if (pointX1.second)
				changeScaleColor(3);
			if (pointX2.second)
				changeScaleColor(0);
			if (pointY1.second)
				changeScaleColor(7);
			if (pointY2.second)
				changeScaleColor(4);
		}
	}

	return QPair<QPointF, QPointF>(QPointF(x1, y1), QPointF(x2, y2));
}

QPair<QPointF, QPointF> XmlLoader::readLineOfXandY(const QDomElement &docItem)
{
	initListScalePoint();
	QPair<QString, bool> pointX1 = readScaleCoord("x1", docItem);
	QPair<QString, bool> pointX2 = readScaleCoord("x2", docItem);
	QPair<QString, bool> pointY1 = readScaleCoord("y1", docItem);
	QPair<QString, bool> pointY2 = readScaleCoord("y2", docItem);
	return calcLineOfXandY(pointX1, pointX2, pointY1, pointY2);
}

QPair<QPointF, QPointF> XmlLoader::readLinePortOfXandY(const QDomElement &start, const QDomElement &end)
{
	initListScalePoint();
	QPair<QString, bool> pointX1 = readScaleCoord("startx", start);
	QPair<QString, bool> pointX2 = readScaleCoord("endx", end);
	QPair<QString, bool> pointY1 = readScaleCoord("starty", start);
	QPair<QString, bool> pointY2 = readScaleCoord("endy", end);
	return calcLineOfXandY(pointX1, pointX2, pointY1, pointY2);
}

QPointF XmlLoader::readXandY(const QDomElement &docItem)
{
	initListScalePoint();
	QPair<QString, bool> pointX = readScaleCoord("x", docItem);
	QPair<QString, bool> pointY = readScaleCoord("y", docItem);

	qreal x = pointX.first.toDouble() + mDrift.x();
	qreal y = pointY.first.toDouble() + mDrift.y();

	checkScale(pointX, QPair<QString, bool>("", false), pointY, QPair<QString, bool>("", false));
	return QPointF(x, y);
}

void XmlLoader::readLine(const QDomElement &line)
{
	QPair<QPointF, QPointF> rect = readLineOfXandY(line);
	Line* item = new Line(rect.first.x(), rect.first.y(), rect.second.x(), rect.second.y(), nullptr);
	item->readPenBrush(line);
	item->setListScalePoint(mListScalePoint);
	item->setVisibilityCondition(readVisibility(line));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readEllipse(const QDomElement &ellipse)
{
	QRectF rect = readRectOfXandY(ellipse);
	QRealEllipse* item = new QRealEllipse(rect.left(), rect.top(), rect.right(), rect.bottom(), nullptr);
	item->readPenBrush(ellipse);
	item->setListScalePoint(mListScalePoint);
	item->setVisibilityCondition(readVisibility(ellipse));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readArch(const QDomElement &arch)
{
	QRectF rect = readRectOfXandY(arch);
	int spanAngle = arch.attribute("spanAngle", "0").toInt();
	int startAngle = arch.attribute("startAngle", "0").toInt();
	Arch* item = new Arch(rect, startAngle, spanAngle, nullptr);
	item->setVisibilityCondition(readVisibility(arch));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readRectangle(const QDomElement &rectangle)
{
	QRectF rect = readRectOfXandY(rectangle);
	QRealRectangle* item = new QRealRectangle(rect.left(), rect.top(), rect.right(), rect.bottom(), nullptr);
	item->readPenBrush(rectangle);
	item->setListScalePoint(mListScalePoint);
	item->setVisibilityCondition(readVisibility(rectangle));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readImage(const QDomElement &image)
{
	QRectF rect = readRectOfXandY(image);
	QString fileName = image.attribute("name", "error");
	const QString workingDirName = QFileInfo(QApplication::applicationFilePath()).absoluteDir().absolutePath();
	const QString fullFileName = workingDirName + "/" + fileName;
	Image* item = new Image(fullFileName, rect.left(), rect.top(), nullptr);
	item->setX2(rect.right());
	item->setY2(rect.bottom());
	item->setListScalePoint(mListScalePoint);
	item->setVisibilityCondition(readVisibility(image));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readStylus(const QDomElement &stylus)
{
	QDomNodeList stylusAttributes = stylus.childNodes();

	Stylus* stylusItem = new Stylus(0, 0, nullptr);
	for (int i = 0; i < stylusAttributes.length(); ++i) {
		QDomElement type = stylusAttributes.at(i).toElement();
		if (type.tagName() == "line") {
			QRectF rect = readRectOfXandY(type);
			Line* item = new Line(rect.left(), rect.top(), rect.right(), rect.bottom(), nullptr);
			item->readPenBrush(type);
			item->setVisibilityCondition(readVisibility(type));
			stylusItem->addLineInList(item);
			stylusItem->setPen(item->pen());
			stylusItem->setBrush(item->brush());
		} else {
			qDebug() << "Incorrect stylus tag";
		}
	}

	stylusItem->setVisibilityCondition(readVisibility(stylus));
	mScene->addItem(stylusItem);
	mScene->setZValue(stylusItem);
}

bool XmlLoader::isNotLCMZ(QString str, int i)
{
	return (i != str.length()) && (str[i] != 'L') && (str[i] != 'C') && (str[i] != 'M')
		&& (str[i] != 'Z');
}

void XmlLoader::readPath(const QDomElement &element)
{
	QString tempStr;
	QPointF endPoint;
	QPointF c1;
	QPointF c2;
	QDomElement elem = element;
	QPainterPath path;

	if (!elem.isNull()) {
		QString dCont;
		dCont = elem.attribute("d").remove(0, 1);
		dCont.append(" Z");

		for (int i = 0; i < dCont.length() - 1;) {
			int j = 0;
			if (dCont[i] == 'M') {
				j = i + 2;
				while (isNotLCMZ(dCont, j)) {
					while (dCont[j] != ' ') {
						tempStr.append(dCont[j]);
						++j;
					}

					endPoint.setX(tempStr.toFloat());
					tempStr.clear();
					++j;

					while (dCont[j] != ' ') {
						tempStr.append(dCont[j]);
						++j;
					}

					endPoint.setY(tempStr.toFloat());
					++j;
					tempStr.clear();
				}

				path.moveTo(endPoint);
				i = j;
			}
			else if (dCont[i] == 'L') {
				j = i + 2;
				while (isNotLCMZ(dCont, j)) {
					while (dCont[j] != ' ') {
						tempStr.append(dCont[j]);
						++j;
					}

					endPoint.setX(tempStr.toFloat());
					tempStr.clear();
					++j;

					while (dCont[j] != ' ') {
						tempStr.append(dCont[j]);
						++j;
					}
					endPoint.setY(tempStr.toFloat());
					++j;
					tempStr.clear();
				}

				 path.lineTo(endPoint);
				 i = j;
			}
			else if (dCont[i] == 'C') {
				j = i + 2;
				while(isNotLCMZ(dCont, j)) {
					while (!(dCont[j] == ' ')) {
						tempStr.append(dCont[j]);
						++j;
					}

					c1.setX(tempStr.toFloat());
					tempStr.clear();
					++j;

					while (dCont[j] != ' ') {
						tempStr.append(dCont[j]);
						++j;
					}

					c1.setY(tempStr.toFloat());
					tempStr.clear();
					++j;

					while (dCont[j] != ' ') {
						tempStr.append(dCont[j]);
						++j;
					}

					c2.setX(tempStr.toFloat());
					tempStr.clear();
					++j;

					while (dCont[j] != ' ') {
						tempStr.append(dCont[j]);
						++j;
					}

					c2.setY(tempStr.toFloat());
					tempStr.clear();
					++j;

					while (dCont[j] != ' ') {
						tempStr.append(dCont[j]);
						++j;
					}

					endPoint.setX(tempStr.toFloat());
					tempStr.clear();
					++j;

					while (dCont[j] != ' ') {
						tempStr.append(dCont[j]);
						++j;
					}

					endPoint.setY(tempStr.toFloat());
					tempStr.clear();
					++j;
				}

				path.cubicTo(c1, c2, endPoint);
				i = j;

			} else if (dCont[i] == 'Z') {
				path.closeSubpath();
			}
		}
	}

	Path *item = new Path(path);
	item->setTransform(QTransform::fromTranslate(mDrift.x(), mDrift.y()), true);
	item->readPenBrush(elem);
	item->setVisibilityCondition(readVisibility(elem));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readCurve(const QDomElement &curve)
{
	QDomNodeList curveAttributes = curve.childNodes();
	qreal x1 = 0;
	qreal y1 = 0;
	qreal x2 = 0;
	qreal y2 = 0;
	qreal x3 = 0;
	qreal y3 = 0;
	for (int i = 0; i < curveAttributes.length(); ++i) {
		QDomElement type = curveAttributes.at(i).toElement();
		if (type.tagName() == "start") {
			x1 = (type.attribute("startx", "0")).toDouble() + mDrift.x();
			y1 = (type.attribute("starty", "0")).toDouble() + mDrift.y();
		} else if (type.tagName() == "end") {
			x2 = (type.attribute("endx", "0")).toDouble() + mDrift.x();
			y2 = (type.attribute("endy", "0")).toDouble() + mDrift.y();
		} else if (type.tagName() == "ctrl") {
			x3 = (type.attribute("x", "0")).toDouble() + mDrift.x();
			y3 = (type.attribute("y", "0")).toDouble() + mDrift.y();
		} else {
			qDebug() << "Incorrect curve tag";
		}
	}

	Curve* item = new Curve(QPointF(x1, y1), QPointF(x2, y2), QPointF(x3, y3));
	item->readPenBrush(curve);
	item->setVisibilityCondition(readVisibility(curve));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readText(const QDomElement &text)
{
	QRectF point = readRectOfXandY(text);
	qreal x = point.left();
	qreal y = point.top();
	QString str = text.text();
	TextPicture* item = new TextPicture(x, y, str);
	item->readFont(text);
	item->setListScalePoint(mListScalePoint);
	item->setVisibilityCondition(readVisibility(text));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readLabel(const QDomElement &label)
{
	QPointF point = readXandY(label);
	Text* item = nullptr;
	if (label.hasAttribute("text")) {
		item = new Text(static_cast<int>(point.x()), static_cast<int>(point.y()), label.attribute("text", ""));
		item->setIsDynamicText(false);
	} else if (label.hasAttribute("textBinded")) {
		item = new Text(static_cast<int>(point.x()), static_cast<int>(point.y()), label.attribute("textBinded", ""));
		item->setIsDynamicText(true);
	} else {
		qDebug() << "Incorrect label tag";
	}

	item->setListScalePoint(mListScalePoint);
	item->setVisibilityCondition(readVisibility(label));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readLinePort(const QDomElement &linePort)
{
	QDomNodeList linePortAttributes = linePort.childNodes();
	QDomElement start;
	QDomElement end;

	for (int i = 0; i < linePortAttributes.length(); ++i) {
		QDomElement type = linePortAttributes.at(i).toElement();
		if (type.tagName() == "start") {
			start = type;
		} else if (type.tagName() == "end") {
			end = type;
		} else {
			qDebug() << "Incorrect linePort tag";
		}
	}

	QPair<QPointF, QPointF> rect = readLinePortOfXandY(start, end);
	LinePort* item = new LinePort(rect.first.x(), rect.first.y(), rect.second.x(), rect.second.y(), nullptr);
	item->setListScalePoint(mListScalePoint);
	item->setVisibilityCondition(readVisibility(linePort));
	item->setType(linePort.attribute("type", "NonTyped"));
	mScene->addItem(item);
	mScene->setZValue(item);
}

void XmlLoader::readPointPort(const QDomElement &pointPort)
{
	QPointF point = readXandY(pointPort);
	PointPort* item = new PointPort(point.x(), point.y(), nullptr);
	item->setListScalePoint(mListScalePoint);
	item->setVisibilityCondition(readVisibility(pointPort));
	item->setType(pointPort.attribute("type", "NonTyped"));
	mScene->addItem(item);
	mScene->addItem(item);
	mScene->setZValue(item);
}

Item::VisibilityCondition XmlLoader::readVisibility(const QDomElement &item)
{
	QDomElement visibility = item.elementsByTagName("showIf").item(0).toElement();
	if (visibility.isNull()) {
		return Item::VisibilityCondition();
	}

	Item::VisibilityCondition result;
	result.property = visibility.attribute("property");
	result.sign = visibility.attribute("sign");
	result.value = visibility.attribute("value");

	return result;
}
