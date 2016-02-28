/* Copyright 2007-2016 QReal Research Group
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

#include <QtCore/QList>
#include <QtCore/QPair>

#include <qrgui/models/edgeInfo.h>
#include <metaMetaModel/linkShape.h>

#include "qrgui/editor/element.h"
#include "qrgui/editor/private/edgeArrangeCriteria.h"

namespace qReal {

class EdgeElementType;

namespace gui {
namespace editor {

class NodeElement;

class LineFactory;
class LineHandler;

/// Represents an instance of some edge element on diagram.
/// Edge elements can connect nodes, be reshaped by mouse, render some text on them.
class QRGUI_EDITOR_EXPORT EdgeElement : public Element
{
	Q_OBJECT

public:
	static const int stripeWidth = 25;

	enum DragType {
		wholeEdge = -2
		, noPort = -1
	};

	enum NodeSide {
		left
		, top
		, right
		, bottom
	};

	EdgeElement(const EdgeElementType &type, const Id &id, const models::Models &models);
	~EdgeElement() override;

	void updateData();

	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	bool initPossibleEdges() override;
	void initTitles() override;

	bool isDividable();

	/// Adjust link to make its' ends be placed exactly on corresponding ports
	void adjustLink();

	/// Reconnect, arrange links on linear ports and lay out the link depending on its' type
	void layOut();

	/// Reconnect link to exclude intersections with adjacent nodes
	void reconnectToNearestPorts(bool reconnectSrc = true, bool reconnectDst = true);

	NodeElement *src() const;
	NodeElement *dst() const;
	void setSrc(NodeElement *node);
	void setDst(NodeElement *node);

	/// Returns true if edge is not connected to port from one side (no matter src or dst).
	bool isHanging() const;

	/// prepare edge to moving from the linker
	void tuneForLinker();

	QPair<qreal, qreal> portIdOn(const NodeElement *node) const;

	/// @return numeric criteria for sorting links on linear ports
	EdgeArrangeCriteria arrangeCriteria(const NodeElement *node, const QLineF &portLine) const;

	NodeElement* otherSide(const NodeElement *node) const;
	void removeLink(const NodeElement *from);

	QPolygonF line() const;
	void setLine(const QPolygonF &line);

	qreal fromPort() const;
	qreal toPort() const;
	void setFromPort(const qreal fromPort);
	void setToPort(const qreal toPort);

	QStringList fromPortTypes() const;
	QStringList toPortTypes() const;

	void placeStartTo(const QPointF &place);
	void placeEndTo(const QPointF &place);
	void moveConnection(NodeElement *node, const qreal portId);

	/// Resort edges connected to linear ports of adjacent nodes
	void arrangeLinearPorts();

	virtual void connectToPort();

	QList<PossibleEdge> getPossibleEdges();

	virtual void setColorRect(bool bl);

	void breakPointHandler(const QPointF &pos);
	bool isBreakPointPressed();
	void breakPointUnpressed();

	void highlight(const QColor &color = Qt::red);

	EdgeInfo data();

	/// Change link type and redraw it
	void changeShapeType(const LinkShape shapeType);

	/// Save link position to the repo
	void setGraphicApiPos();

	/// Save link configuration to the repo
	void saveConfiguration();

	bool isLoop();

	/// Create an edge connected with both ends to the same node
	void createLoopEdge();

	/// Connect link to nearest newMaster's ports
	void connectLoopEdge(NodeElement *newMaster);

	/// @return Node at position that is more appropriate for the link to connect to.
	NodeElement *getNodeAt(const QPointF &position, bool isStart);

	/// Determine on which side of a node (top, bottom, right or left) the link's end is placed
	NodeSide defineNodePortSide(bool isStart) const;

	/// Align link's intermediate points to grid
	void alignToGrid();

	/// Proxies QGraphicsItem`s setPos filtering out NaNs
	void setPos(qreal x, qreal y);

	/// Proxies QGraphicsItem`s setPos filtering out NaNs
	void setPos(const QPointF &pos);

protected:
	void paint(QPainter* p, const QStyleOptionGraphicsItem *opt, QWidget* w) override;

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	void drawStartArrow(QPainter *painter) const;
	void drawEndArrow(QPainter *painter) const;

	Qt::PenStyle mPenStyle;
	int mPenWidth;
	QColor mPenColor;

private slots:
	void reverse();

private:
	void initLineHandler();

	int indentReductCoeff();

	/// Set mPortTo to next port.
	void searchNextPort();

	/// Create indent of bounding rect, depending on the rect size.
	QPointF boundingRectIndent(const QPointF &point, NodeSide direction);

	/// Returns true, if the sides adjacent.
	bool isNeighbor(const NodeSide &startSide, const NodeSide &endSide) const;

	/// Returns the next clockwise side.
	NodeSide rotateRight(NodeSide side) const;

	void paintEdge(QPainter *painter, const QStyleOptionGraphicsItem *option, bool drawSavedLine) const;
	void drawArrows(QPainter *painter, bool savedLine) const;
	QPen edgePen(QPainter *painter, QColor color, Qt::PenStyle style, int width) const;
	void setEdgePainter(QPainter *painter, QPen pen, qreal opacity) const;

	NodeElement *innermostChild(const QList<QGraphicsItem *> &items, NodeElement * const element) const;
	void updateLongestPart();

	bool reverseActionIsPossible() const;
	bool canConnect(const NodeElement * const node, bool from) const;
	void reversingReconnectToPorts(NodeElement *newSrc, NodeElement *newDst);

	const EdgeElementType &mType;

	QList<PossibleEdge> mPossibleEdges;

	NodeElement *mSrc;
	NodeElement *mDst;

	LineFactory *mLineFactory; // Takes ownership
	LineHandler *mHandler; // Takes ownership

	qreal mPortFrom;
	qreal mPortTo;

	LinkShape mShapeType;

	int mDragType; // is a number of mLine's point we're trying to drag

	int mLongPart;

	QPolygonF mLine; // holds coordinates of polygon points in coordinate system with center in first point
	QColor mColor;

	ContextMenuAction mReverseAction;
	ContextMenuAction mChangeShapeAction;

	bool mBreakPointPressed;

	bool mModelUpdateIsCalled;  // flag for the infinite updateData()-s liquidating

	bool mIsLoop; // if line is self-closing (mSrc == mDst && mDst)
};

}
}
}
