#include "image.h"
#include <QtGui/QBitmap>
#include <QtGui/QImageWriter>

Image::Image(QString fileName, qreal x, qreal y, Item* parent)
	: Item(parent)
	, mImage(fileName)
	, mRectangleImpl()
{
	mFileName = fileName;
	QPixmap* pixmap = new QPixmap(fileName);
	mPixmapItem = new QGraphicsPixmapItem(*pixmap);
	mNeedScalingRect = true;
	mDomElementType = pictureType;
	mX1 = x;
	mY1 = y;
	mX2 = x + pixmap->width();
	mY2 = y + pixmap->height();
}

Image::Image(Image const &other)
	:Item(), mRectangleImpl()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = pictureType;
	mX1 = other.mX1;
	mX2 = other.mX2;
	mY1 = other.mY1;
	mY2 = other.mY2;
	mListScalePoint = other.mListScalePoint;
	mPixmapItem = other.mPixmapItem;
	mFileName = other.mFileName;
	mImage = other.mImage;
	setPos(other.x(), other.y());
}

Item* Image::clone()
{
	Image* item = new Image(*this);
	return item;
}

QRectF Image::boundingRect() const
{
	return mRectangleImpl.boundingRect(mX1, mY1, mX2, mY2, scalingDrift);
}

void Image::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	mRectangleImpl.drawImageItemWithMirrored(painter, mX1, mY1, mX2, mY2, mImage);
}

void Image::setItemZValue(int zValue)
{
	mZValue = zValue;
	mPixmapItem->setZValue(zValue);
	setZValue(zValue);
}

QPair<QDomElement, Item::DomElementTypes> Image::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement image = document.createElement("image");
	setXandY(image, sceneBoundingRectCoord(topLeftPicture));
	image.setAttribute("name", "images/" + mFileName.section('/', -1));

	return QPair<QDomElement, Item::DomElementTypes>(image, mDomElementType);
}
