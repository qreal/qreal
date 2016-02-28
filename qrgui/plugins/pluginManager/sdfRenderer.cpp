/* Copyright 2007-2016 QReal Research Group
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

#include "sdfRenderer.h"

#include <QtCore/QLineF>
#include <QtCore/QTime>
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtWidgets/QApplication>
#include <QtGui/QFont>
#include <QtGui/QIcon>

#include <qrkernel/platformInfo.h>

using namespace qReal;

SdfRenderer::SdfRenderer()
	: mStartX(0), mStartY(0), mNeedScale(true), mElementRepo(0)
{
	mWorkingDirName = SettingsManager::value("workingDir").toString();
}

SdfRenderer::SdfRenderer(const QString path)
	: mStartX(0), mStartY(0), mNeedScale(true)
{
	if (!load(path))
	{
		qDebug() << "File " + path + " - loading failed!";
	}
	mWorkingDirName = SettingsManager::value("workingDir").toString();
}

SdfRenderer::~SdfRenderer()
{
}

bool SdfRenderer::load(const QString &filename)
{
	if (filename.isEmpty()) {
		return false;
	}

	QFile file(filename);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}
	file.close();

	QDomElement docElem = doc.documentElement();
	first_size_x = docElem.attribute("sizex").toInt();
	first_size_y = docElem.attribute("sizey").toInt();

	return true;
}

bool SdfRenderer::load(const QDomDocument &document)
{
	doc = document;
	const QDomElement docElem = doc.firstChildElement("picture");
	first_size_x = docElem.attribute("sizex").toInt();
	first_size_y = docElem.attribute("sizey").toInt();

	return true;
}

void SdfRenderer::setElementRepo(ElementRepoInterface *elementRepo)
{
	mElementRepo = elementRepo;
}

void SdfRenderer::setZoom(qreal zoomFactor)
{
	mZoom = zoomFactor;
}

void SdfRenderer::render(QPainter *painter, const QRectF &bounds, bool isIcon)
{
	current_size_x = static_cast<int>(bounds.width());
	current_size_y = static_cast<int>(bounds.height());
	mStartX = static_cast<int>(bounds.x());
	mStartY = static_cast<int>(bounds.y());
	this->painter = painter;
	QDomElement docElem = doc.documentElement();
	QDomNode node = docElem.firstChild();
	while(!node.isNull())
	{
		QDomElement elem = node.toElement();
		if(!elem.isNull())
		{
			if (!checkShowConditions(elem, isIcon)) {
				node = node.nextSibling();
				continue;
			}
			if (elem.tagName()=="line")
			{
				line(elem);
			}
			else if(elem.tagName()=="ellipse")
			{
				ellipse(elem);
			}
			else if (elem.tagName() == "arc") {
				arc(elem);
			}
			else if(elem.tagName()=="background")
			{
				background(elem);
			}
			else if(elem.tagName()=="text")
			{
				draw_text(elem);
			}
			else if (elem.tagName()=="rectangle")
			{
				rectangle(elem);
			}
			else if (elem.tagName()=="polygon")
			{
				polygon(elem);
			}
			else if (elem.tagName()=="point")
			{
				point(elem);
			}
			else if(elem.tagName()=="path")
			{
				path_draw(elem);
			}
			else if(elem.tagName()=="stylus")
			{
				stylus_draw(elem);
			}
			else if(elem.tagName()=="curve")
			{
				curve_draw(elem);
			}
			else if(elem.tagName()=="image")
			{
				image_draw(elem);
			}
		}
		node = node.nextSibling();
	}
	this->painter = 0;
}

bool SdfRenderer::checkShowConditions(const QDomElement &element, bool isIcon) const
{
	QDomNodeList showConditions = element.elementsByTagName("showIf");
	// a hack, need to be removed when there is another version of icons
	if (!showConditions.isEmpty() && isIcon) {
		return false;
	}
	if (showConditions.isEmpty() || !mElementRepo) {
		return true;
	}
	for (int i = 0; i < showConditions.length(); ++i) {
		if (!checkCondition(showConditions.at(i).toElement())) {
			return false;
		}
	}
	return true;
}

bool SdfRenderer::checkCondition(const QDomElement &condition) const
{
	QString sign = condition.attribute("sign");
	QString realValue = mElementRepo->logicalProperty(condition.attribute("property"));
	QString conditionValue = condition.attribute("value");

	if (sign == "=~") {
		return QRegExp(conditionValue).exactMatch(realValue);
	} else if (sign == ">") {
		return realValue.toInt() > conditionValue.toInt();
	} else if (sign == "<") {
		return realValue.toInt() < conditionValue.toInt();
	} else if (sign == ">=") {
		return realValue.toInt() >= conditionValue.toInt();
	} else if (sign == "<=") {
		return realValue.toInt() <= conditionValue.toInt();
	} else if (sign == "!=") {
		return realValue != conditionValue;
	} else if (sign == "=") {
		return realValue == conditionValue;
	} else {
		qDebug() << "Unsupported logical operator \"" + sign + "\"";
		return false;
	}
}

void SdfRenderer::line(QDomElement &element)
{
	float x1 = x1_def(element);
	float y1 = y1_def(element);
	float x2 = x2_def(element);
	float y2 = y2_def(element);
	QLineF line (x1,y1,x2,y2);

	parsestyle(element);
	painter->drawLine(line);
}

void SdfRenderer::ellipse(QDomElement &element)
{
	float x1 = x1_def(element);
	float y1 = y1_def(element);
	float x2 = x2_def(element);
	float y2 = y2_def(element);

	QRectF rect(x1, y1, x2-x1, y2-y1);
	parsestyle(element);
	painter->drawEllipse(rect);
}

void SdfRenderer::arc(QDomElement &element)
{
	float x1 = x1_def(element);
	float y1 = y1_def(element);
	float x2 = x2_def(element);
	float y2 = y2_def(element);
	int startAngle = element.attribute("startAngle").toInt();
	int spanAngle = element.attribute("spanAngle").toInt();

	QRectF rect(x1, y1, x2-x1, y2-y1);
	parsestyle(element);
	painter->drawArc(rect, startAngle, spanAngle);
}

void SdfRenderer::background(QDomElement &element)
{
	parsestyle(element);
	painter->setPen(brush.color());
	painter->drawRect(painter->window());
	defaultstyle();
}

void SdfRenderer::draw_text(QDomElement &element)
{
	parsestyle(element);
	pen.setStyle(Qt::SolidLine);
	painter->setPen(pen);
	float x1 = x1_def(element);
	float y1 = y1_def(element);
	QString str = element.text();

	// delete "\n" from the beginning of the string
	if (str[0] == '\n')
		str.remove(0, 1);

	// delete "\n" from the end of the string
	if (str[str.length() - 1] == '\n')
		str.remove(str.length() - 1, 1);

	while (str.contains('\n'))
	{
		int i = str.indexOf('\n');
		QString temp = str.left(i);
		str.remove(0, i + 1);
		painter->drawText(static_cast<int>(x1), static_cast<int>(y1), temp);
		y1 += painter->font().pixelSize() ;
	}
	QPointF point(x1, y1);
	painter->drawText(point, str);
	defaultstyle();
}

void SdfRenderer::rectangle(QDomElement &element)
{
	float x1 = x1_def(element);
	float y1 = y1_def(element);
	float x2 = x2_def(element);
	float y2 = y2_def(element);

	QRectF rect;
	rect.adjust(x1, y1, x2, y2);
	parsestyle(element);
	painter->drawRect(rect);
	defaultstyle();
}

void SdfRenderer::polygon(QDomElement &element)
{
	parsestyle(element);
	// FIXME: init points array here
	QPoint *points = nullptr;
	int n = element.attribute("n").toInt();
	if (!element.isNull())
	{
		points = getpoints(element, n);
	}
	if (points != nullptr)
	{
		painter->drawConvexPolygon(points, n);
		delete[] points;
	}
	defaultstyle();
}

void SdfRenderer::image_draw(QDomElement &element)
{
	float const x1 = x1_def(element);
	float const y1 = y1_def(element);
	float const x2 = x2_def(element);
	float const y2 = y2_def(element);

	const QString fileName = SettingsManager::value("pathToImages").toString() + "/"
			+ element.attribute("name", "default");

	const QRect rect(x1, y1, x2 - x1, y2 - y1);
	ImagesCache::instance().drawImage(fileName, *painter, rect, mZoom);
}

void SdfRenderer::point(QDomElement &element)
{
	parsestyle(element);
	float x = x1_def(element);
	float y = y1_def(element);
	QPointF pointf(x,y);
	painter->drawLine(QPointF(pointf.x()-0.1, pointf.y()-0.1), QPointF(pointf.x()+0.1, pointf.y()+0.1));
	defaultstyle();
}

QPoint *SdfRenderer::getpoints(QDomElement &element, int n)
{
	QPoint *array = new QPoint[n];
	float x = 0;
	float y = 0;
	for (int i = 0; i < n; i++)
	{
		QString str;
		str.setNum(i + 1);
		QDomElement elem = element;
		QString xnum = elem.attribute(QString("x").append(str));
		if (xnum.endsWith("%"))
		{
			xnum.chop(1);
			x = current_size_x * xnum.toFloat() / 100 + mStartX;
		}
		else if (xnum.endsWith("a") && mNeedScale)
		{
			xnum.chop(1);
			x = xnum.toFloat() + mStartX;
		}
		else if (xnum.endsWith("a") && !mNeedScale)
		{
			xnum.chop(1);
			x = xnum.toFloat() * current_size_x / first_size_x + mStartX;
		}
		else
			x = xnum.toFloat() * current_size_x / first_size_x + mStartX;

		QString ynum = elem.attribute(QString("y").append(str));
		if (ynum.endsWith("%"))
		{
			ynum.chop(1);
			y = current_size_y * ynum.toFloat() / 100 + mStartY;
		}
		else if (ynum.endsWith("a") && mNeedScale)
		{
			ynum.chop(1);
			y = ynum.toFloat() + mStartY;
		}
		else if (ynum.endsWith("a") && !mNeedScale)
		{
			ynum.chop(1);
			y = ynum.toFloat() * current_size_y / first_size_y + mStartY;
		}
		else
			y = ynum.toFloat() * current_size_y / first_size_y + mStartY;

		array[i].setX(static_cast<int>(x));
		array[i].setY(static_cast<int>(y));
	}
	return array;
}

void SdfRenderer::defaultstyle()
{
	pen.setColor(QColor(0,0,0));
	brush.setColor(QColor(255,255,255));
	pen.setStyle(Qt::SolidLine);
	brush.setStyle(Qt::NoBrush);
	pen.setWidth(1);
}

bool SdfRenderer::isNotLCMZ(QString str, int i)
{
	return (str[i] != 'L') && (str[i] != 'C') && (str[i] != 'M')
		&& (str[i] != 'Z') && (i != str.length());
}

void SdfRenderer::path_draw(QDomElement &element)
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

					end_point.setX(s1.toFloat() * current_size_x / first_size_x + mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					end_point.setY(s1.toFloat() * current_size_y / first_size_y + mStartY);
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

					end_point.setX(s1.toFloat() * current_size_x / first_size_x + mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}
					end_point.setY(s1.toFloat() * current_size_y / first_size_y + mStartY);
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

					c1.setX(s1.toFloat() * current_size_x / first_size_x + mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					c1.setY(s1.toFloat() * current_size_y / first_size_y + mStartY);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					c2.setX(s1.toFloat() * current_size_x / first_size_x + mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					c2.setY(s1.toFloat() * current_size_y / first_size_y + mStartY);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					end_point.setX(s1.toFloat() * current_size_x / first_size_x + mStartX);
					s1.clear();
					++j;

					while (d_cont[j] != ' ')
					{
						s1.append(d_cont[j]);
						++j;
					}

					end_point.setY(s1.toFloat() * current_size_y / first_size_y + mStartY);
					s1.clear();
					++j;
				}

				path.cubicTo(c1, c2, end_point);
				i = j;

			} else if (d_cont[i] == 'Z')
			{
				path.closeSubpath();
				logger ("loggerZ.txt", "DONE");
			}
		}
	}

	parsestyle(element);
	painter->drawPath(path);
}

void SdfRenderer::stylus_draw(QDomElement &element)
{
	QDomNode node = element.firstChild();
	while(!node.isNull())
	{
		QDomElement elem = node.toElement();
		if(!elem.isNull())
		{
			if (elem.tagName()=="line")
			{
				line(elem);
			}
		}
		node = node.nextSibling();
	}
}

void SdfRenderer::curve_draw(QDomElement &element)
{
	QDomNode node = element.firstChild();
	QPointF start(0, 0);
	QPointF end(0, 0);
	QPoint c1(0, 0);
	while(!node.isNull())
	{
		QDomElement elem = node.toElement();
		if(!elem.isNull())
		{
			if (elem.tagName() == "start")
			{
				start.setX(elem.attribute("startx").toDouble() * current_size_x / first_size_x);
				start.setY(elem.attribute("starty").toDouble() * current_size_y / first_size_y);
			}
			else if (elem.tagName() == "end")
			{
				end.setX(elem.attribute("endx").toDouble() * current_size_x / first_size_x);
				end.setY(elem.attribute("endy").toDouble() * current_size_y / first_size_y);
			}
			else if (elem.tagName() == "ctrl")
			{
				c1.setX(elem.attribute("x").toDouble() * current_size_x / first_size_x);
				c1.setY(elem.attribute("y").toDouble() * current_size_y / first_size_y);
			}
		}
		node = node.nextSibling();
	}

	QPainterPath path(start);
	path.quadTo(c1, end);
	parsestyle(element);
	painter->drawPath(path);
}

void SdfRenderer::parsestyle(QDomElement &element)
{
	QDomElement elem = element;
	if(!elem.isNull())
	{
		if (elem.hasAttribute("stroke-width"))
		{
			if (mNeedScale)
				pen.setWidth(elem.attribute("stroke-width").toInt());
			else  // for painting icons. width of all lines should be set to 1
				pen.setWidth(1);
		}

		if (elem.hasAttribute("fill"))
		{
			QColor color = elem.attribute("fill");
			brush.setStyle(Qt::SolidPattern);
			brush.setColor(color);
		}

		if (elem.hasAttribute("stroke"))
		{
			QColor color = elem.attribute("stroke");
			pen.setColor(color);
		}

		if (elem.hasAttribute("stroke-style"))
		{
			if (elem.attribute("stroke-style") == "solid")
				pen.setStyle(Qt::SolidLine);
			if (elem.attribute("stroke-style") == "dot")
				pen.setStyle(Qt::DotLine);
			if (elem.attribute("stroke-style") == "dash")
				pen.setStyle(Qt::DashLine);
			if (elem.attribute("stroke-style") == "dashdot")
				pen.setStyle(Qt::DashDotLine);
			if (elem.attribute("stroke-style") == "dashdotdot")
				pen.setStyle(Qt::DashDotDotLine);
			if (elem.attribute("stroke-style") == "none")
				pen.setStyle(Qt::NoPen);
		}

		if (elem.hasAttribute("fill-style"))
		{
			if (elem.attribute("fill-style")=="none")
				brush.setStyle(Qt::NoBrush);
			else if(elem.attribute("fill-style")=="solid")
				brush.setStyle(Qt::SolidPattern);
		}

		if (elem.hasAttribute("font-fill"))
		{
			QColor color = elem.attribute("font-fill");
			pen.setColor(color);
		}

		if (elem.hasAttribute("font-size"))
		{
			QString fontsize = elem.attribute("font-size");
			if (fontsize.endsWith("%"))
			{
				fontsize.chop(1);
				font.setPixelSize(current_size_y * fontsize.toInt() / 100);
			}
			else if (fontsize.endsWith("a") && mNeedScale)
			{
				fontsize.chop(1);
				font.setPixelSize(fontsize.toInt());
			}
			else if (fontsize.endsWith("a") && !mNeedScale)
			{
				fontsize.chop(1);
				font.setPixelSize(fontsize.toInt() * current_size_y / first_size_y);
			}
			else
				font.setPixelSize(fontsize.toInt() * current_size_y / first_size_y);
		}

		if (elem.hasAttribute("font-name"))
		{
			font.setFamily(elem.attribute("font-name"));
		}

		if (elem.hasAttribute("b"))
		{
			font.setBold(elem.attribute("b").toInt());
		}

		if (elem.hasAttribute("i"))
		{
			font.setItalic(elem.attribute("i").toInt());
		}

		if (elem.hasAttribute("u"))
		{
			font.setUnderline(elem.attribute("u").toInt());
		}

		painter->setFont(font);
	}
	painter->setPen(pen);
	painter->setBrush(brush);
}

float SdfRenderer::coord_def(QDomElement &element, QString coordName, int current_size, int first_size)
{
	float coord = 0;
	QString coordStr = element.attribute(coordName);

	if (coordStr.endsWith("%"))
	{
		coordStr.chop(1);
		coord = current_size * coordStr.toFloat() / 100;
		return coord;
	}
	else if (coordStr.endsWith("a") && mNeedScale)
	{
		coordStr.chop(1);
		coord = coordStr.toFloat();
		return coord;
	}
	else if (coordStr.endsWith("a") && !mNeedScale)
	{
		coordStr.chop(1);
		coord = coordStr.toFloat() * current_size / first_size;
		return coord;
	}
	else
	{
		coord = coordStr.toFloat() * current_size / first_size;
		return coord;
	}
}

float SdfRenderer::x1_def(QDomElement &element)
{
	return coord_def(element, "x1", current_size_x, first_size_x) + mStartX;
}

float SdfRenderer::y1_def(QDomElement &element)
{
	return coord_def(element, "y1", current_size_y, first_size_y) + mStartY;
}

float SdfRenderer::x2_def(QDomElement &element)
{
	return coord_def(element, "x2", current_size_x, first_size_x) + mStartX;
}

float SdfRenderer::y2_def(QDomElement &element)
{
	return coord_def(element, "y2", current_size_y, first_size_y) + mStartY;
}

void SdfRenderer::logger(QString path, QString string)
{	log.setFileName(path);
	log.open(QIODevice::Append | QIODevice::Text);
	logtext.setDevice(&log);
	logtext << string << "\n";
	log.close();
}

void SdfRenderer::noScale()
{
	mNeedScale = false;
}

SdfRenderer::ImagesCache::ImagesCache()
{
}

SdfRenderer::ImagesCache::~ImagesCache()
{
}

SdfRenderer::ImagesCache &SdfRenderer::ImagesCache::instance()
{
	static ImagesCache instance;
	return instance;
}

void SdfRenderer::ImagesCache::drawImage(const QString &fileName, QPainter &painter, const QRect &rect, qreal zoom)
{
	if (mFileNamePixmapMap.contains(fileName)) {
		painter.drawPixmap(rect, mFileNamePixmapMap.value(fileName));
		return;
	}

	QTransform scale;
	scale.scale(zoom, zoom);
	auto scaledRect = scale.mapRect(rect);
	auto savePrerenderedSvg = [this, &fileName, &scaledRect] (QSvgRenderer &renderer) {
		QPixmap pixmap(scaledRect.size());
		pixmap.fill(Qt::transparent);
		QPainter pixmapPainter(&pixmap);
		renderer.render(&pixmapPainter, scaledRect.translated(-scaledRect.topLeft()));
		mPrerenderedSvgs[fileName].insert(scaledRect, pixmap);
	};

	if (mFileNameSvgRendererMap.contains(fileName)) {
		if (!mPrerenderedSvgs.contains(fileName) || !mPrerenderedSvgs[fileName].contains(scaledRect)) {
			savePrerenderedSvg(*mFileNameSvgRendererMap.value(fileName));
		}

		painter.drawPixmap(rect, mPrerenderedSvgs[fileName][scaledRect]);
	} else {
		// Cache miss - finding best file to load and loading it.
		const QFileInfo actualFile = selectBestImageFile(PlatformInfo::invariantPath(fileName));
		const QByteArray rawImage = loadPixmap(actualFile);

		if (actualFile.suffix() == "svg") {
			QSharedPointer<QSvgRenderer> renderer(new QSvgRenderer(rawImage));
			mFileNameSvgRendererMap.insert(fileName, renderer);
			savePrerenderedSvg(*renderer);
			painter.drawPixmap(rect, mPrerenderedSvgs[fileName][scaledRect]);
		} else {
			QPixmap pixmap;
			pixmap.loadFromData(rawImage);
			mFileNamePixmapMap.insert(fileName, pixmap);
			painter.drawPixmap(rect, pixmap);
		}
	}
}

QFileInfo SdfRenderer::ImagesCache::selectBestImageFile(const QString &filePath)
{
	const QFileInfo originalFileInfo(filePath);
	const QFileInfo svgVersion(originalFileInfo.path() + originalFileInfo.completeBaseName() + "svg");

	if (svgVersion.exists()) {
		return svgVersion;
	}

	const QFileInfo fileInfo(filePath);
	if (fileInfo.exists() && fileInfo.isFile()) {
		return fileInfo;
	}

	QDir dir(fileInfo.absolutePath());
	auto candidates = dir.entryInfoList({fileInfo.completeBaseName() + ".*"}, QDir::Files);
	if (!candidates.empty()) {
		return candidates.at(0);
	}

	if (fileInfo.completeBaseName() != "default") {
		return selectBestImageFile(fileInfo.absolutePath() + "/default.svg");
	}

	return QFileInfo(":/pluginManager/images/default.svg");
}

QByteArray SdfRenderer::ImagesCache::loadPixmap(const QFileInfo &fileInfo)
{
	QFile file(fileInfo.absoluteFilePath());
	if (!file.open(QIODevice::ReadOnly)) {
		return QByteArray();
	}

	return file.readAll();
}


SdfIconEngineV2::SdfIconEngineV2(const QString &file)
{
	mRenderer.load(file);
	mRenderer.noScale();
	mSize = QSize(mRenderer.pictureWidth(), mRenderer.pictureHeight());
}

SdfIconEngineV2::SdfIconEngineV2(const QDomDocument &document)
{
	mRenderer.load(document);
	mRenderer.noScale();
}

void SdfIconEngineV2::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
{
	Q_UNUSED(mode)
	Q_UNUSED(state)
	painter->eraseRect(rect);
	int rh = rect.height();
	int rw = rect.width();

	int ph = mRenderer.pictureHeight();
	int pw = mRenderer.pictureWidth();
	if (pw == 0 || ph == 0) { // SUDDENLY!!11
		return;
	}
	// Take picture aspect into account
	QRect resRect = rect;
	if (rh * pw < ph * rw) {
		resRect.setLeft(rect.left() + (rw - rh * pw / ph) / 2);
		resRect.setRight(rect.right() - (rw - rh * pw / ph) / 2);
	}
	if (rh * pw > ph * rw) {
		resRect.setTop(rect.top() + (rh - rw * ph / pw) / 2);
		resRect.setBottom(rect.bottom() - (rh - rw * ph / pw) / 2);
	}
	painter->setRenderHint(QPainter::Antialiasing, true);
	mRenderer.render(painter, resRect, true);
}

QIconEngine *SdfIconEngineV2::clone() const
{
	return nullptr;
}

QSize SdfIconEngineV2::preferedSize() const
{
	return mSize;
}


QIcon SdfIconLoader::iconOf(const QString &fileName)
{
	return loadPixmap(fileName);
}

QSize SdfIconLoader::preferedSizeOf(const QString &fileName)
{
	loadPixmap(fileName);
	return instance()->mPreferedSizes[fileName];
}

SdfIconLoader *SdfIconLoader::instance()
{
	static SdfIconLoader instance;
	return &instance;
}

SdfIconLoader::SdfIconLoader()
{
}

SdfIconLoader::~SdfIconLoader()
{
}

QIcon SdfIconLoader::loadPixmap(const QString &fileName)
{
	if (!instance()->mLoadedIcons.contains(fileName)) {
		SdfIconEngineV2 * const engine = new SdfIconEngineV2(fileName);
		// QIcon takes ownership over SdfIconEngineV2
		QIcon icon(engine);
		instance()->mLoadedIcons[fileName] = icon;
		instance()->mPreferedSizes[fileName] = engine->preferedSize();
	}

	return instance()->mLoadedIcons[fileName];
}
