#include "unrealDisplayImplementation.h"
#include "../details/nxtDisplay.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::displayImplementations;
using namespace graphicsUtils;

UnrealDisplayImplementation::UnrealDisplayImplementation(d2Model::D2RobotModel *d2Model)
	: mD2Model(d2Model)
{
}

void UnrealDisplayImplementation::attachToPaintWidget()
{
	mD2Model->display()->setPainter(this);
}

void UnrealDisplayImplementation::read()
{
	emit response(mD2Model->display()->leftButtonIsDown()
		, mD2Model->display()->rightButtonIsDown()
		, mD2Model->display()->centralButtonIsDown()
		, mD2Model->display()->bottomButtonIsDown());
}

void UnrealDisplayImplementation::drawPixel(int x, int y)
{
	mPoints << QPoint(x, y);
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::drawRect(int x, int y, int width, int height)
{
	mRects << QRect(x, y, width, height);
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::clearScreen()
{
	mPoints.clear();
	mLines.clear();
	mRects.clear();
	mCircles.clear();
	mStrings.clear();
	mStringPlaces.clear();
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::drawLine(int x1, int y1, int x2, int y2)
{
	mLines << QLine(x1, y1, x2, y2);
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::drawCircle(int x, int y, int radius)
{
	mCircles << QRect(x - radius, y - radius, 2 * radius, 2 * radius);
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::printText(int x, int y, QString const &text)
{
	mStringPlaces << QPoint(x, y);
	mStrings << text;
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::paint(QPainter *painter)
{
	qreal const pixWidth = static_cast<qreal>(mD2Model->display()->displayWidth()) / nxtDisplayWidth;
	qreal const pixHeight = static_cast<qreal>(mD2Model->display()->displayHeight()) / nxtDisplayHeight;

	QPen pen;
	QFont font;
	font.setPixelSize(pixHeight * textPixelHeight);

	painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
	foreach (QPoint const &point, mPoints) {
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
		QString const str = strings.next();
		QPoint const place = strPlaces.next();
		painter->drawText(place.x() * pixWidth * nxtDisplayWidth / textPixelWidth
				, place.y() * pixHeight * nxtDisplayHeight / textPixelHeight, str);
	}
}
