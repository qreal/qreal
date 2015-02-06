#include "twoDDisplay.h"

using namespace nxtKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;

Display::Display(const DeviceInfo &info
		, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::NxtDisplay(info, port)
	, mEngine(engine)
{
	mEngine.display()->setPainter(this);
}

void Display::drawPixel(int x, int y)
{
	mPoints << QPoint(x, y);
	mEngine.display()->repaintDisplay();
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	mLines << QLine(x1, y1, x2, y2);
	mEngine.display()->repaintDisplay();
}

void Display::drawRect(int x, int y, int width, int height)
{
	mRects << QRect(x, y, width, height);
	mEngine.display()->repaintDisplay();
}

void Display::drawCircle(int x, int y, int radius)
{
	mCircles << QRect(x - radius, y - radius, 2 * radius, 2 * radius);
	mEngine.display()->repaintDisplay();
}

void Display::printText(int x, int y, const QString &text)
{
	mStringPlaces << QPoint(x, y);
	mStrings << text;
	mEngine.display()->repaintDisplay();
}

void Display::clearScreen()
{
	mPoints.clear();
	mLines.clear();
	mRects.clear();
	mCircles.clear();
	mStrings.clear();
	mStringPlaces.clear();
	mEngine.display()->repaintDisplay();
}

void Display::paint(QPainter *painter)
{
	/// @todo ZOMG.
	qreal const pixWidth = static_cast<qreal>(mEngine.display()->displayWidth()) / nxtDisplayWidth;
	qreal const pixHeight = static_cast<qreal>(mEngine.display()->displayHeight()) / nxtDisplayHeight;

	QPen pen;
	QFont font;
	font.setPixelSize(pixHeight * textPixelHeight);

	painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
	foreach (const QPoint &point, mPoints) {
		painter->drawRect(point.x() * pixWidth, point.y() * pixHeight, pixWidth, pixHeight);
	}

	pen.setWidth((pixWidth + pixHeight) / 2);
	painter->setPen(pen);
	painter->setBrush(QBrush(Qt::black, Qt::NoBrush));
	painter->setFont(font);

	foreach (QLine const &line, mLines) {
		painter->drawLine(line.x1() * pixWidth, line.y1() * pixHeight, line.x2() * pixWidth, line.y2() * pixHeight);
	}

	foreach (QRect const &circle, mCircles) {
		painter->drawEllipse(circle.x() * pixWidth, circle.y() * pixHeight, circle.width() * pixWidth, circle.height() * pixHeight);
	}

	foreach (QRect const &rect, mRects) {
		painter->drawRect(rect.x() * pixWidth, rect.y() * pixHeight, rect.width() * pixWidth, rect.height() * pixHeight);
	}

	QListIterator<QString> strings(mStrings);
	QListIterator<QPoint> strPlaces(mStringPlaces);
	while (strings.hasNext() && strPlaces.hasNext()) {
		const QString str = strings.next();
		const QPoint place = strPlaces.next();
		painter->drawText(place.x() * pixWidth * nxtDisplayWidth / textPixelWidth
				, place.y() * pixHeight * nxtDisplayHeight / textPixelHeight, str);
	}
}

void Display::clear()
{
	clearScreen();
}
