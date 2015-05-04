/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtWidgets/QGraphicsScene>

#include "abstractItemView.h"
#include "abstractItem.h"
#include "qrutils/utilsDeclSpec.h"

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

	static bool compareItems(graphicsUtils::AbstractItem* first, graphicsUtils::AbstractItem* second);
	QString convertPenToString(const QPen &pen);
	QString convertBrushToString(const QBrush &brush);
	void setPenBrushItems(const QPen &pen, const QBrush &brush);
	void setEmptyPenBrushItems();

	void setPenStyleItems(const QString &text);
	void setPenWidthItems(int width);
	void setPenColorItems(const QString &text);
	void setBrushStyleItems(const QString &text);
	void setBrushColorItems(const QString &text);

	/// Appends the given action to the context menu shown on empty scene.
	void addAction(QAction * const action);

	/// Appends the given list of action to the context menu shown on empty scene.
	void addActions(const QList<QAction *> &actions);

signals:
	/// Emitted when user pressed left mouse button on this scene.
	void leftButtonPressed();

	/// Emitted when user releases left mouse button on this scene.
	void leftButtonReleased();

protected:
	void setEmptyRect(int x, int y, int w, int h);

	void setX1andY1(QGraphicsSceneMouseEvent *event);
	void setX2andY2(QGraphicsSceneMouseEvent *event);
	void reshapeItem(QGraphicsSceneMouseEvent *event);
	void reshapeItem(QGraphicsSceneMouseEvent *event, graphicsUtils::AbstractItem *item);

	void removeMoveFlag(QGraphicsSceneMouseEvent *event, QGraphicsItem* item);
	void setMoveFlag(QGraphicsSceneMouseEvent *event);

	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

	QString penStyleItems();
	int penWidthItems();
	QString penColorItems();
	QString brushStyleItems();
	QString brushColorItems();

	QList<AbstractItem *> abstractItems(const QPointF &scenePos) const;

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

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

	QList<QAction *> mActions;
};

}
