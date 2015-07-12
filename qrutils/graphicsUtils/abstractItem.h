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

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsView>
#include <QtXml/QDomDocument>

#include "qrutils/utilsDeclSpec.h"

const int drift = 15;
const int resizeDrift = 10;
const int scalingDrift = 10;
const int scalingRect = 6;

namespace graphicsUtils {

class QRUTILS_EXPORT AbstractItem : public QGraphicsObject
{
	Q_OBJECT

public:
	enum DragState {
		None
		, TopLeft
		, TopRight
		, BottomLeft
		, BottomRight
		, Ctrl
	};

	explicit AbstractItem(QGraphicsItem *parent = 0);

	virtual QRectF boundingRect() const = 0;
	virtual QRectF realBoundingRect() const;
	virtual QRectF calcNecessaryBoundingRect() const;
	virtual QPainterPath realShape() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) = 0;
	virtual void drawExtractionForItem(QPainter *painter);
	virtual void drawFieldForResizeItem(QPainter *painter);
	virtual void setPenBrushForExtraction(QPainter *painter, const QStyleOptionGraphicsItem *option);
	virtual void setPenBrushDriftRect(QPainter *painter);

	static QStringList getPenStyleList();
	static QStringList getBrushStyleList();

	QPen pen() const;
	QBrush brush() const;
	void setBrush(const QBrush &brush);
	void setPen(const QPen &pen);

	virtual void setPenStyle(const QString &text);
	virtual void setPenWidth(int width);
	virtual void setPenColor(const QString &text);
	virtual void setBrushStyle(const QString &text);
	virtual void setBrushColor(const QString &text);
	virtual void setBrush(const QString &brushStyle, const QString &brushColor);
	virtual void setPen(const QString &penStyle, int width, const QString &penColor);
	virtual void setPenBrush(const QString &penStyle, int width, const QString &penColor, const QString &brushStyle
			, const QString &brushColor);

	/// Returns a x-coordinate of the first item`s end (it may be one corner of the rectangle or the begin of the line).
	qreal x1() const;
	/// Returns a y-coordinate of the first item`s end (it may be one corner of the rectangle or the begin of the line).
	qreal x2() const;
	/// Returns a x-coordinate of the second item`s end (it may be one corner of the rectangle or the end of the line).
	qreal y1() const;
	/// Returns a y-coordinate of the second item`s end (it may be one corner of the rectangle or the end of the line).
	qreal y2() const;
	/// Sets an x-coordinate of the first item`s end (it may be one corner of the rectangle or the begin of the line).
	void setX1(qreal x1);
	/// Sets an y-coordinate of the first item`s end (it may be one corner of the rectangle or the begin of the line).
	void setY1(qreal y1);
	/// Sets an x-coordinate of the second item`s end (it may be one corner of the rectangle or the end of the line).
	void setX2(qreal x2);
	/// Sets an y-coordinate of the second item`s end (it may be one corner of the rectangle or the end of the line).
	void setY2(qreal y2);

	void setCoordinates(const QRectF &pos);

	virtual void reshapeRectWithShift();
	virtual void changeDragState(qreal x, qreal y);
	DragState dragState() const;
	void setDragState(DragState dragState);

	virtual void calcResizeItem(QGraphicsSceneMouseEvent *event);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);
	void reverseOldResizingItem(const QPointF &begin, const QPointF &end);

	//for save to xml
	virtual void setXandY(QDomElement& dom, const QRectF &rect);
	QDomElement setPenBrushToDoc(QDomDocument &document, const QString &domName) const;
	virtual QRectF sceneBoundingRectCoord(const QPoint &topLeftPicture);
	void readPenBrush(const QDomElement &docItem);

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

	/// Returns a unique identifier of an item if it was assigned in the model.
	QString id() const;

	/// Sets a unique identifier of an item.
	void setId(const QString &id);

	void setEditable(bool editable);
	bool editable() const;

signals:
	/// Emitted when QGraphicsItem has repositioned.
	void positionChanged(const QPointF &pos);

	/// Emitted when the x-coorinate of the first item`s end modified for some reason.
	void x1Changed(qreal x1);

	/// Emitted when the x-coorinate of the first item`s end modified for some reason.
	void y1Changed(qreal y1);

	/// Emitted when the x-coorinate of the first item`s end modified for some reason.
	void x2Changed(qreal x2);

	/// Emitted when the x-coorinate of the first item`s end modified for some reason.
	void y2Changed(qreal y2);

	/// Emitted when item`s pen changed somehow.
	void penChanged(const QPen &pen);

	/// Emitted when item`s brush changed somehow.
	void brushChanged(const QBrush &brush);

	/// Emitted when user clicks with right mouse button on this item and selects to remove it.
	void deletedWithContextMenu();

protected:
	virtual void serialize(QDomElement &element) const;
	virtual void deserialize(const QDomElement &element);

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	void copyTo(AbstractItem * const other) const;

private:
	DragState mDragState;
	QPen mPen;
	QBrush mBrush;
	qreal mX1;
	qreal mY1;
	qreal mX2;
	qreal mY2;
	QString mId;
	bool mEditable;
};

}
