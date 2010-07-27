#include "path.h"
#include <QtCore/QTextStream>

Path::Path(QPainterPath const &path) : Item(NULL)
{
	mNeedScalingRect = false;
	mPath = path;
	mDomElementType = Item::pictureType;
}

Path::Path(Path const &other)
	:Item()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = Item::pictureType;
	mX1 = other.mX1;
	mX2 = other.mX2;
	mY1 = other.mY1;
	mY2 = other.mY2;
	mPath = other.mPath;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* Path::clone()
{
	Path* item = new Path(*this);
	return item;
}

QRectF Path::boundingRect() const
{
	return mPath.boundingRect();
}

void Path::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->drawPath(mPath);
}

void Path::drawExtractionForItem(QPainter* painter)
{
	Q_UNUSED(painter);
	drawFieldForResizeItem(painter);
}

void Path::drawFieldForResizeItem(QPainter* painter)
{
	Q_UNUSED(painter);
}

void Path::drawScalingRects(QPainter* painter)
{
	Q_UNUSED(painter);
}

QPair<QDomElement, Item::DomElementTypes> Path::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{

	QDomElement path = setPenBrushToDoc(document, "path");

	int count = mPath.elementCount();
	QPainterPath::Element elem;
	path.setAttribute("int", count);

	QString text = "";
	for (int i = 0; i < count; ++i)
	{
		elem = mPath.elementAt(i);
		if (elem.type == QPainterPath::LineToElement)
		{
			text += " L ";
			text += QString::number(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString::number(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
		else if (elem.type == QPainterPath::CurveToElement) {
			text += " C ";
			text += QString::number(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString::number(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
		else if (elem.type == QPainterPath::MoveToElement)
		{
			text += " M ";
			text += QString::number(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString::number(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
		else if (elem.type == QPainterPath::CurveToDataElement)
		{
			text += " ";
			text += QString::number(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString::number(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
	}
	path.setAttribute("d", text);

	return QPair<QDomElement, Item::DomElementTypes>(path, mDomElementType);
}
