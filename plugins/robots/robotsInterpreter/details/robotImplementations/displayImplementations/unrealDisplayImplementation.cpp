#include <QMutableListIterator>

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
				  ,mD2Model->display()->rightButtonIsDown()
				  ,mD2Model->display()->centralButtonIsDown()
				  ,mD2Model->display()->bottomButtonIsDown());
}

void UnrealDisplayImplementation::drawPixel(int x, int y)
{
	QPoint p;
	p.setX(x);
	p.setY(y);
	mPoints.append(p);
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::drawRect(int x, int y, int width, int height)
{
	QRect r;
	r.setX(x);
	r.setY(y);
	r.setWidth(width);
	r.setHeight(height);
	mRects.append(r);
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
	QLine l;\
	l.setLine(x1,y1,x2,y2);
	mLines.append(l);
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::drawCircle(int x, int y, int radius)
{
	QRect c;
	c.setX(x-radius);
	c.setY(y-radius);
	c.setHeight(2*radius);
	c.setWidth(2*radius);
	mCircles.append(c);
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::printText(int x, int y, QString text)
{
	QString s = text;
	QPoint p;
	p.setX(x);
	p.setY(y);
	mStringPlaces.append(p);
	mStrings.append(s);
	mD2Model->display()->repaintDisplay();
}

void UnrealDisplayImplementation::paint(QPainter *painter)
{
	//painter->drawLine(10,10,30,30);
	double pixWidth = (double) mD2Model->display()->displayWidth() / 100;
	double pixHeight = (double) mD2Model->display()->displayHeight() / 64;

	QPen q;
	QFont f;
	f.setPixelSize(pixHeight*8);

	painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
	QMutableListIterator<QPoint> points(mPoints);

	while(points.hasNext()){
		QPoint p = points.next();
		painter->drawRect(p.x()*pixWidth, p.y()*pixHeight, pixWidth, pixHeight);
	}

	q.setWidth((pixWidth+pixHeight)/2);
	painter->setPen(q);
	painter->setBrush(QBrush(Qt::black, Qt::NoBrush));
	painter->setFont(f);


	QMutableListIterator<QLine> lines(mLines);
	while(lines.hasNext()){
		QLine l = lines.next();
		painter->drawLine(l.x1()*pixWidth, l.y1()*pixHeight, l.x2()*pixWidth, l.y2()*pixHeight);
	}

	QMutableListIterator<QRect> circles(mCircles);
	while(circles.hasNext()){
		QRect c = circles.next();
		painter->drawEllipse(c.x()*pixWidth, c.y()*pixHeight, c.width()*pixWidth, c.height()*pixHeight);
	}

	QMutableListIterator<QRect> rects(mRects);
	while(rects.hasNext()){
		QRect r = rects.next();
		painter->drawRect(r.x()*pixWidth, r.y()*pixHeight, r.width()*pixWidth, r.height()*pixHeight);
	}

	QMutableListIterator<QString> strings(mStrings);
	QMutableListIterator<QPoint> strPlaces(mStringPlaces);
	while(strings.hasNext() && strPlaces.hasNext()){
		QString s = strings.next();
		QPoint p = strPlaces.next();
		painter->drawText(p.x()*pixWidth, p.y()*pixHeight, s);
	}


}
