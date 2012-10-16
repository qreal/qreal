#include "sdfRenderer.h"

#include <QtGui/QMessageBox>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtCore/QLineF>
#include <QtCore/QTime>
#include <QtCore/QDebug>

using namespace qReal;

SdfRenderer::SdfRenderer()
	: mStartX(0), mStartY(0), mNeedScale(true)
{
	initWorkingDir();
}

SdfRenderer::SdfRenderer(const QString path)
	: mStartX(0), mStartY(0), mNeedScale(true)
{
	if (!load(path)) {
		qDebug() << "File " + path + " - loading failed!";
	}
	initWorkingDir();
}

SdfRenderer::SdfRenderer(const QDomDocument &document)
	: mStartX(0), mStartY(0), mNeedScale(true)
{
	load(document);
	initWorkingDir();
}

SdfRenderer::~SdfRenderer()
{
}

bool SdfRenderer::load(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}

	if (!mDoc.setContent(&file)) {
		file.close();
		return false;
	}
	file.close();

	initFirstSizes();

	return true;
}

void SdfRenderer::load(const QDomDocument &document)
{
	mDoc = document;
	initFirstSizes();
}

void SdfRenderer::initFirstSizes()
{
	QDomElement docElem = mDoc.documentElement();
	mFirstSizeX = docElem.attribute("sizex").toInt();
	mFirstSizeY = docElem.attribute("sizey").toInt();
}

void SdfRenderer::initWorkingDir()
{
	mWorkingDirName = SettingsManager::value("workingDir").toString();
}

void SdfRenderer::render(QPainter *painter, const QRectF &bounds)
{
	mCurrentSizeX = static_cast<int>(bounds.width());
	mCurrentSizeY = static_cast<int>(bounds.height());
	mStartX = static_cast<int>(bounds.x());
	mStartY = static_cast<int>(bounds.y());
	this->mPainter = painter;
	QDomElement docElem = mDoc.documentElement();
	QDomNode node = docElem.firstChild();
	while(!node.isNull()) {
		QDomElement elem = node.toElement();
		if(!elem.isNull()) {
			if (elem.tagName()=="line") {
				line(elem);
			} else if(elem.tagName()=="ellipse") {
				ellipse(elem);
			} else if (elem.tagName() == "arc") {
				arc(elem);
			} else if(elem.tagName()=="background") {
				background(elem);
			} else if(elem.tagName()=="text") {
				draw_text(elem);
			} else if (elem.tagName()=="rectangle") {
				rectangle(elem);
			} else if (elem.tagName()=="polygon") {
				polygon(elem);
			} else if (elem.tagName()=="point") {
				point(elem);
			} else if(elem.tagName()=="path") {
				path_draw(elem);
			} else if(elem.tagName()=="stylus") {
				stylus_draw(elem);
			} else if(elem.tagName()=="curve") {
				curve_draw(elem);
			} else if(elem.tagName()=="image") {
				image_draw(elem);
			}
		}
		node = node.nextSibling();
	}
	this->mPainter = 0;
}

void SdfRenderer::line(QDomElement &element)
{
	qreal const x1 = x1_def(element);
	qreal const y1 = y1_def(element);
	qreal const x2 = x2_def(element);
	qreal const y2 = y2_def(element);
	QLineF line (x1,y1,x2,y2);

	parsestyle(element);
	mPainter->drawLine(line);
}

void SdfRenderer::ellipse(QDomElement &element)
{
	qreal const x1 = x1_def(element);
	qreal const y1 = y1_def(element);
	qreal const x2 = x2_def(element);
	qreal const y2 = y2_def(element);

	QRectF rect(x1, y1, x2-x1, y2-y1);
	parsestyle(element);
	mPainter->drawEllipse(rect);
}

void SdfRenderer::arc(QDomElement &element)
{
	qreal const x1 = x1_def(element);
	qreal const y1 = y1_def(element);
	qreal const x2 = x2_def(element);
	qreal const y2 = y2_def(element);
	int const startAngle = element.attribute("startAngle").toInt();
	int const spanAngle = element.attribute("spanAngle").toInt();

	QRectF rect(x1, y1, x2 - x1, y2 - y1);
	parsestyle(element);
	mPainter->drawArc(rect, startAngle, spanAngle);
}

void SdfRenderer::background(QDomElement &element)
{
	parsestyle(element);
	mPainter->setPen(mBrush.color());
	mPainter->drawRect(mPainter->window());
	defaultstyle();
}

void SdfRenderer::draw_text(QDomElement &element)
{
	parsestyle(element);
	mPen.setStyle(Qt::SolidLine);
	mPainter->setPen(mPen);
	qreal const x1 = x1_def(element);
	qreal y1 = y1_def(element);
	QString str = element.text();

	// delete "\n" from the beginning of the string
	if (str[0] == '\n') {
		str.remove(0, 1);
	}

	// delete "\n" from the end of the string
	if (str[str.length() - 1] == '\n') {
		str.remove(str.length() - 1, 1);
	}

	while (str.contains('\n')) {
		int const i = str.indexOf('\n');
		QString temp = str.left(i);
		str.remove(0, i + 1);
		mPainter->drawText(static_cast<int>(x1), static_cast<int>(y1), temp);
		y1 += mPainter->font().pixelSize();
	}
	QPointF point(x1, y1);
	mPainter->drawText(point, str);
	defaultstyle();
}

void SdfRenderer::rectangle(QDomElement &element)
{
	qreal const x1 = x1_def(element);
	qreal const y1 = y1_def(element);
	qreal const x2 = x2_def(element);
	qreal const y2 = y2_def(element);

	QRectF rect;
	rect.adjust(x1, y1, x2, y2);
	parsestyle(element);
	mPainter->drawRect(rect);
	defaultstyle();
}

void SdfRenderer::polygon(QDomElement &element)
{
	parsestyle(element);
	// FIXME: init points array here
	QPoint *points = NULL;
	int const n = element.attribute("n").toInt();
	if (!element.isNull()) {
		points = getpoints(element, n);
	}
	if (points) {
		mPainter->drawConvexPolygon(points, n);
		delete[] points;
	}
	defaultstyle();
}

void SdfRenderer::image_draw(QDomElement &element)
{
	qreal const x1 = x1_def(element);
	qreal const y1 = y1_def(element);
	qreal const x2 = x2_def(element);
	qreal const y2 = y2_def(element);
	QString const fileName = SettingsManager::value("pathToImages").toString() + "/" + element.attribute("name", "error");

	QPixmap pixmap;

	if(mMapFileImage.contains(fileName)) {
		pixmap = mMapFileImage.value(fileName);
	} else {
		pixmap = QPixmap(fileName);
		mMapFileImage.insert(fileName, pixmap);
	}
	QRect rect(x1, y1, x2-x1, y2-y1);
	mPainter->drawPixmap(rect, pixmap);
}

void SdfRenderer::point(QDomElement &element)
{
	parsestyle(element);
	qreal const x = x1_def(element);
	qreal const y = y1_def(element);
	QPointF pointf(x,y);
	mPainter->drawLine(QPointF(pointf.x() - 0.1, pointf.y() - 0.1)
			, QPointF(pointf.x() + 0.1, pointf.y() + 0.1));
	defaultstyle();
}

QPoint *SdfRenderer::getpoints(QDomElement &element, int n)
{
	QPoint *array = new QPoint[n];
	qreal x = 0;
	qreal y = 0;
	for (int i = 0; i < n; ++i) {
		QString str;
		str.setNum(i + 1);
		QDomElement elem = element;
		QString xnum = elem.attribute(QString("x").append(str));
		if (xnum.endsWith("%")) {
			xnum.chop(1);
			x = mCurrentSizeX * xnum.toFloat() / 100 + mStartX;
		} else if (xnum.endsWith("a") && mNeedScale) {
			xnum.chop(1);
			x = xnum.toFloat() + mStartX;
		} else if (xnum.endsWith("a") && !mNeedScale) {
			xnum.chop(1);
			x = xnum.toFloat() * mCurrentSizeX / mFirstSizeX + mStartX;
		} else {
			x = xnum.toFloat() * mCurrentSizeX / mFirstSizeX + mStartX;
		}

		QString ynum = elem.attribute(QString("y").append(str));
		if (ynum.endsWith("%")) {
			ynum.chop(1);
			y = mCurrentSizeY * ynum.toFloat() / 100 + mStartY;
		} else if (ynum.endsWith("a") && mNeedScale) {
			ynum.chop(1);
			y = ynum.toFloat() + mStartY;
		} else if (ynum.endsWith("a") && !mNeedScale) {
			ynum.chop(1);
			y = ynum.toFloat() * mCurrentSizeY / mFirstSizeY + mStartY;
		} else {
			y = ynum.toFloat() * mCurrentSizeY / mFirstSizeY + mStartY;
		}

		array[i].setX(static_cast<int>(x));
		array[i].setY(static_cast<int>(y));
	}
	return array;
}

void SdfRenderer::defaultstyle()
{
	mPen.setColor(QColor(0,0,0));
	mBrush.setColor(QColor(255,255,255));
	mPen.setStyle(Qt::SolidLine);
	mBrush.setStyle(Qt::NoBrush);
	mPen.setWidth(1);
}

bool SdfRenderer::isNotLCMZ(const QString &str, int i)
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

	if (!elem.isNull()) {
		QString d_cont;
		d_cont = elem.attribute("d").remove(0, 1);
		d_cont.append(" Z");

		for (i = 0; i < d_cont.length() - 1;) {
			if (d_cont[i] == 'M') {
				j = i + 2;
				while (isNotLCMZ(d_cont, j)) {
					while (d_cont[j] != ' ') {
						mS1.append(d_cont[j]);
						++j;
					}

					end_point.setX(mS1.toFloat() * mCurrentSizeX / mFirstSizeX + mStartX);
					mS1.clear();
					++j;

					while (d_cont[j] != ' ') {
						mS1.append(d_cont[j]);
						++j;
					}

					end_point.setY(mS1.toFloat() * mCurrentSizeY / mFirstSizeY + mStartY);
					++j;
					mS1.clear();
				}

				path.moveTo(end_point);
				i = j;
			} else if (d_cont[i] == 'L') {
				j = i + 2;
				while (isNotLCMZ(d_cont, j)) {
					while (d_cont[j] != ' ') {
						mS1.append(d_cont[j]);
						++j;
					}

					end_point.setX(mS1.toFloat() * mCurrentSizeX / mFirstSizeX + mStartX);
					mS1.clear();
					++j;

					while (d_cont[j] != ' ') {
						mS1.append(d_cont[j]);
						++j;
					}
					end_point.setY(mS1.toFloat() * mCurrentSizeY / mFirstSizeY + mStartY);
					++j;
					mS1.clear();
				}

				 path.lineTo(end_point);
				 i = j;
			} else if (d_cont[i] == 'C') {
				j = i + 2;
				while(isNotLCMZ(d_cont, j)) {
					while (!(d_cont[j] == ' ')) {
						mS1.append(d_cont[j]);
						++j;
					}

					c1.setX(mS1.toFloat() * mCurrentSizeX / mFirstSizeX + mStartX);
					mS1.clear();
					++j;

					while (d_cont[j] != ' ') {
						mS1.append(d_cont[j]);
						++j;
					}

					c1.setY(mS1.toFloat() * mCurrentSizeY / mFirstSizeY + mStartY);
					mS1.clear();
					++j;

					while (d_cont[j] != ' ') {
						mS1.append(d_cont[j]);
						++j;
					}

					c2.setX(mS1.toFloat() * mCurrentSizeX / mFirstSizeX + mStartX);
					mS1.clear();
					++j;

					while (d_cont[j] != ' ') {
						mS1.append(d_cont[j]);
						++j;
					}

					c2.setY(mS1.toFloat() * mCurrentSizeY / mFirstSizeY + mStartY);
					mS1.clear();
					++j;

					while (d_cont[j] != ' ') {
						mS1.append(d_cont[j]);
						++j;
					}

					end_point.setX(mS1.toFloat() * mCurrentSizeX / mFirstSizeX + mStartX);
					mS1.clear();
					++j;

					while (d_cont[j] != ' ') {
						mS1.append(d_cont[j]);
						++j;
					}

					end_point.setY(mS1.toFloat() * mCurrentSizeY / mFirstSizeY + mStartY);
					mS1.clear();
					++j;
				}

				path.cubicTo(c1, c2, end_point);
				i = j;

			} else if (d_cont[i] == 'Z') {
				path.closeSubpath();
				logger ("loggerZ.txt", "DONE");
			}
		}
	}

	parsestyle(element);
	mPainter->drawPath(path);
}

void SdfRenderer::stylus_draw(QDomElement &element)
{
	QDomNode node = element.firstChild();
	while(!node.isNull()) {
		QDomElement elem = node.toElement();
		if (!elem.isNull()) {
			if (elem.tagName()=="line") {
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
	while(!node.isNull()) {
		QDomElement elem = node.toElement();
		if(!elem.isNull()) {
			if (elem.tagName() == "start") {
				start.setX(elem.attribute("startx").toDouble() * mCurrentSizeX / mFirstSizeX);
				start.setY(elem.attribute("starty").toDouble() * mCurrentSizeY / mFirstSizeY);
			} else if (elem.tagName() == "end") {
				end.setX(elem.attribute("endx").toDouble() * mCurrentSizeX / mFirstSizeX);
				end.setY(elem.attribute("endy").toDouble() * mCurrentSizeY / mFirstSizeY);
			} else if (elem.tagName() == "ctrl") {
				c1.setX(elem.attribute("x").toDouble() * mCurrentSizeX / mFirstSizeX);
				c1.setY(elem.attribute("y").toDouble() * mCurrentSizeY / mFirstSizeY);
			}
		}
		node = node.nextSibling();
	}

	QPainterPath path(start);
	path.quadTo(c1, end);
	parsestyle(element);
	mPainter->drawPath(path);
}

void SdfRenderer::parsestyle(QDomElement &element)
{
	QDomElement elem = element;
	if(!elem.isNull()) {
		if (elem.hasAttribute("stroke-width")) {
			if (mNeedScale) {
				mPen.setWidth(elem.attribute("stroke-width").toInt());
			} else { // for painting icons. width of all lines should be set to 1
				mPen.setWidth(1);
			}
		}

		if (elem.hasAttribute("fill")) {
			QColor color = elem.attribute("fill");
			mBrush.setStyle(Qt::SolidPattern);
			mBrush.setColor(color);
		}

		if (elem.hasAttribute("stroke")) {
			QColor color = elem.attribute("stroke");
			mPen.setColor(color);
		}

		if (elem.hasAttribute("stroke-style")) {
			if (elem.attribute("stroke-style") == "solid") {
				mPen.setStyle(Qt::SolidLine);
			}
			if (elem.attribute("stroke-style") == "dot") {
				mPen.setStyle(Qt::DotLine);
			}
			if (elem.attribute("stroke-style") == "dash") {
				mPen.setStyle(Qt::DashLine);
			}
			if (elem.attribute("stroke-style") == "dashdot") {
				mPen.setStyle(Qt::DashDotLine);
			}
			if (elem.attribute("stroke-style") == "dashdotdot") {
				mPen.setStyle(Qt::DashDotDotLine);
			}
			if (elem.attribute("stroke-style") == "none") {
				mPen.setStyle(Qt::NoPen);
			}
		}

		if (elem.hasAttribute("fill-style")) {
			if (elem.attribute("fill-style")=="none") {
				mBrush.setStyle(Qt::NoBrush);
			} else if(elem.attribute("fill-style")=="solid") {
				mBrush.setStyle(Qt::SolidPattern);
			}
		}

		if (elem.hasAttribute("font-fill")) {
			QColor color = elem.attribute("font-fill");
			mPen.setColor(color);
		}

		if (elem.hasAttribute("font-size")) {
			QString fontsize = elem.attribute("font-size");
			if (fontsize.endsWith("%")) {
				fontsize.chop(1);
				mFont.setPixelSize(mCurrentSizeY * fontsize.toInt() / 100);
			} else if (fontsize.endsWith("a") && mNeedScale) {
				fontsize.chop(1);
				mFont.setPixelSize(fontsize.toInt());
			} else if (fontsize.endsWith("a") && !mNeedScale) {
				fontsize.chop(1);
				mFont.setPixelSize(fontsize.toInt() * mCurrentSizeY / mFirstSizeY);
			} else {
				mFont.setPixelSize(fontsize.toInt() * mCurrentSizeY / mFirstSizeY);
			}
		}

		if (elem.hasAttribute("font-name")) {
			mFont.setFamily(elem.attribute("font-name"));
		}

		if (elem.hasAttribute("b")) {
			mFont.setBold(elem.attribute("b").toInt());
		}

		if (elem.hasAttribute("i")) {
			mFont.setItalic(elem.attribute("i").toInt());
		}

		if (elem.hasAttribute("u")) {
			mFont.setUnderline(elem.attribute("u").toInt());
		}

		mPainter->setFont(mFont);
	}
	mPainter->setPen(mPen);
	mPainter->setBrush(mBrush);
}

qreal SdfRenderer::coord_def(QDomElement &element, QString const &coordName, int current_size, int first_size)
{
	qreal coord = 0;
	QString coordStr = element.attribute(coordName);

	if (coordStr.endsWith("%")) {
		coordStr.chop(1);
		coord = current_size * coordStr.toFloat() / 100;
		return coord;
	} else if (coordStr.endsWith("a") && mNeedScale) {
		coordStr.chop(1);
		coord = coordStr.toFloat();
		return coord;
	} else if (coordStr.endsWith("a") && !mNeedScale) {
		coordStr.chop(1);
		coord = coordStr.toFloat() * current_size / first_size;
		return coord;
	} else {
		coord = coordStr.toFloat() * current_size / first_size;
		return coord;
	}
}

qreal SdfRenderer::x1_def(QDomElement &element)
{
	return coord_def(element, "x1", mCurrentSizeX, mFirstSizeX) + mStartX;
}

qreal SdfRenderer::y1_def(QDomElement &element)
{
	return coord_def(element, "y1", mCurrentSizeY, mFirstSizeY) + mStartY;
}

qreal SdfRenderer::x2_def(QDomElement &element)
{
	return coord_def(element, "x2", mCurrentSizeX, mFirstSizeX) + mStartX;
}

qreal SdfRenderer::y2_def(QDomElement &element)
{
	return coord_def(element, "y2", mCurrentSizeY, mFirstSizeY) + mStartY;
}

void SdfRenderer::logger(QString const &path, QString const &string)
{
	mLog.setFileName(path);
	mLog.open(QIODevice::Append | QIODevice::Text);
	mLogText.setDevice(&mLog);
	mLogText << string << "\n";
	mLog.close();
}

void SdfRenderer::noScale()
{
	mNeedScale = false;
}


SdfIconEngineV2::SdfIconEngineV2(QString const &file)
{
	mRenderer.load(file);
	mRenderer.noScale();
}

void SdfIconEngineV2::paint(QPainter *painter, QRect const &rect,
	QIcon::Mode mode, QIcon::State state)
{
	Q_UNUSED(mode)
	Q_UNUSED(state)
	painter->eraseRect(rect);
	int const rh = rect.height();
	int const rw = rect.width();

	int const ph = mRenderer.pictureHeight();
	int const pw = mRenderer.pictureWidth();
	if (pw == 0 || ph == 0) { // SUDDENLY!!11
		return;
	}
	// Take picture aspect into account
	QRect resRect = rect;
	if (rh * pw < ph * rw)
	{
		resRect.setLeft(rect.left() + (rw-rh*pw/ph)/2);
		resRect.setRight(rect.right() - (rw-rh*pw/ph)/2);
	}
	if (rh * pw > ph * rw)
	{
		resRect.setTop(rect.top() + (rh-rw*ph/pw)/2);
		resRect.setBottom(rect.bottom() - (rh-rw*ph/pw)/2);
	}
	painter->setRenderHint(QPainter::Antialiasing, true);
	mRenderer.render(painter, resRect);
}
