#pragma once

#include <QtCore/QObject>

#include "../layoutTool.h"

namespace Ui
{
namespace WidgetsEdit
{

class LayoutTool;

class LayoutHelperBase : public QObject
{
	Q_OBJECT

public:
	LayoutHelperBase(LayoutTool *tool);
	virtual ~LayoutHelperBase() {}

	void setDragPos(QPointF const &pos);
	void disconnectHelper();

	virtual void drawCurrentDropPosition(QPainter *painter) = 0;
	virtual void dropItem(Tool *item) = 0;

	virtual void resetLayout(QGraphicsLayout *layout);

	virtual void startChildDrag(Tool *item) = 0;
	virtual void finishChildDrag(bool success) = 0;

	virtual void generateAttachedProperty(QDomElement &element, Tool *tool) = 0;

protected:
	virtual void onToolResized(QRectF const &newGeometrry) = 0;

	LayoutTool *mTool;
	QPointF mPos;

private slots:
	void onGeometryChanged();
};

}
}
