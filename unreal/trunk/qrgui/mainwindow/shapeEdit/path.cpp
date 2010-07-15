#include "path.h"
#include <QtCore/QTextStream>

Path::Path(QPainterPath const &path) : Item(NULL)
{
	mPath = path;
	mDomElementType = Item::pictureType;
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
}

QPair<QDomElement, Item::DomElementTypes> Path::generateItem(QDomDocument &document, QPointF const &topLeftPicture)
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
			text += QString("%1").arg(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString("%1").arg(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
		else if (elem.type == QPainterPath::CurveToElement) {
			text += " C ";
			text += QString("%1").arg(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString("%1").arg(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
		else if (elem.type == QPainterPath::MoveToElement)
		{
			text += " M ";
			text += QString("%1").arg(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString("%1").arg(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
		else if (elem.type == QPainterPath::CurveToDataElement)
		{
			text += " ";
			text += QString("%1").arg(static_cast<float>(elem.x) + scenePos().x() - topLeftPicture.x());
			text += " ";
			text += QString("%1").arg(static_cast<float>(elem.y) + scenePos().y() - topLeftPicture.y());
		}
	}
	path.setAttribute("d", text);

	return QPair<QDomElement, Item::DomElementTypes>(path, mDomElementType);
}
