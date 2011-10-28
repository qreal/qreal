#pragma once
#include <QtGui/QGraphicsItem>
#include "../utilsDeclSpec.h"

namespace graphicsUtils
{

class QRUTILS_EXPORT RotateInterface
{
public:
	virtual void rotate(double angle) = 0;
	virtual QRectF rect() const = 0;
	qreal horizontalRadius() const;
	virtual double rotateAngle() const = 0;
	virtual void setSelected(bool isSelected) = 0;
	virtual void checkSelection() = 0;
protected:
	QPointF mPreviousScenePos;
};

}
