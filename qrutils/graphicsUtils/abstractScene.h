#pragma once

#include <QtWidgets/QGraphicsScene>

#include "abstractItemView.h"
#include "abstractItem.h"
#include "../utilsDeclSpec.h"

namespace graphicsUtils
{

class QRUTILS_EXPORT AbstractScene : public QGraphicsScene
{
	Q_OBJECT

public:
	explicit AbstractScene(AbstractView *view, QObject *parent = 0);
	graphicsUtils::AbstractView *mainView();
	QRect realItemsBoundingRect() const;

	void setDragMode(int itemsType);
	void setDragMode(QGraphicsView::DragMode mode);
	virtual void forPressResize(QGraphicsSceneMouseEvent *event);
	virtual void forMoveResize(QGraphicsSceneMouseEvent *event);
	virtual void forReleaseResize(QGraphicsSceneMouseEvent *event);

	void removeMoveFlag(QGraphicsSceneMouseEvent *event, QGraphicsItem* item);
	void setMoveFlag(QGraphicsSceneMouseEvent *event);

	static bool compareItems(graphicsUtils::AbstractItem* first, graphicsUtils::AbstractItem* second);
	QString convertPenToString(QPen const &pen);
	QString convertBrushToString(QBrush const &brush);
	void setPenBrushItems(QPen const &pen, QBrush const &brush);
	void setEmptyPenBrushItems();

	QString penStyleItems();
	int penWidthItems();
	QString penColorItems();
	QString brushStyleItems();
	QString brushColorItems();

	void setPenStyleItems(QString const &text);
	void setPenWidthItems(int width);
	void setPenColorItems(QString const &text);
	void setBrushStyleItems(QString const &text);
	void setBrushColorItems(QString const &text);

protected:
	AbstractView *mView;
	graphicsUtils::AbstractItem *mGraphicsItem;
	qreal mX1;
	qreal mX2;
	qreal mY1;
	qreal mY2;
	int mSizeEmptyRectX;
	int mSizeEmptyRectY;

	QString mPenStyleItems;
	int mPenWidthItems;
	QString mPenColorItems;
	QString mBrushStyleItems;
	QString mBrushColorItems;

	int mFirstPenWidth;

	QGraphicsRectItem *mEmptyRect;

	void setEmptyRect(int x, int y, int w, int h);

	void setX1andY1(QGraphicsSceneMouseEvent *event);
	void setX2andY2(QGraphicsSceneMouseEvent *event);
	void reshapeItem(QGraphicsSceneMouseEvent *event);
	void reshapeItem(QGraphicsSceneMouseEvent *event, graphicsUtils::AbstractItem *item);
};

}
