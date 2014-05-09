#include "twoDDisplay.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;

Display::Display(DeviceInfo const &info
		, PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikDisplay(info, port)
	, mEngine(engine)
{
	mEngine.display()->setPainter(this);
}

void Display::drawSmile(bool sad)
{
}

void Display::setBackground(QColor const &color)
{
}

void Display::clearScreen()
{
	if (mEngine.display()) {
		mEngine.display()->repaintDisplay();
	}
}

void Display::paint(QPainter *painter)
{
	/// @todo ZOMG.
//	qreal const pixWidth = static_cast<qreal>(mEngine.display()->displayWidth()) / nxtDisplayWidth;
//	qreal const pixHeight = static_cast<qreal>(mEngine.display()->displayHeight()) / nxtDisplayHeight;

//	QPen pen;
//	QFont font;
//	font.setPixelSize(pixHeight * textPixelHeight);

//	painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
//	foreach (QPoint const &point, mPoints) {
//		painter->drawRect(point.x() * pixWidth, point.y() * pixHeight, pixWidth, pixHeight);
//	}

//	pen.setWidth((pixWidth + pixHeight) / 2);
//	painter->setPen(pen);
//	painter->setBrush(QBrush(Qt::black, Qt::NoBrush));
//	painter->setFont(font);

//	foreach (QLine const &line, mLines) {
//		painter->drawLine(line.x1() * pixWidth, line.y1() * pixHeight, line.x2() * pixWidth, line.y2() * pixHeight);
//	}

//	foreach (QRect const &circle, mCircles) {
//		painter->drawEllipse(circle.x() * pixWidth, circle.y() * pixHeight, circle.width() * pixWidth, circle.height() * pixHeight);
//	}

//	foreach (QRect const &rect, mRects) {
//		painter->drawRect(rect.x() * pixWidth, rect.y() * pixHeight, rect.width() * pixWidth, rect.height() * pixHeight);
//	}

//	QListIterator<QString> strings(mStrings);
//	QListIterator<QPoint> strPlaces(mStringPlaces);
//	while (strings.hasNext() && strPlaces.hasNext()) {
//		QString const str = strings.next();
//		QPoint const place = strPlaces.next();
//		painter->drawText(place.x() * pixWidth * nxtDisplayWidth / textPixelWidth
//				, place.y() * pixHeight * nxtDisplayHeight / textPixelHeight, str);
	//	}
}

void Display::clear()
{
	clearScreen();
}
