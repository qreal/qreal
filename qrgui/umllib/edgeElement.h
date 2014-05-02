#pragma once

#include <QtCore/QList>
#include <QtCore/QPair>

#include "umllib/element.h"
#include "editorPluginInterface/elementImpl.h"
#include "umllib/serializationData.h"
#include "umllib/private/edgeArrangeCriteria.h"

namespace qReal {

class NodeElement;

class LineFactory;
class LineHandler;

/** @class EdgeElement
* @brief class for an edge on a diagram
*/

class EdgeElement : public Element
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

	EdgeElement(ElementImpl *impl
			, Id const &id
			, qReal::models::GraphicalModelAssistApi &graphicalAssistApi
			, qReal::models::LogicalModelAssistApi &logicalAssistApi);

	virtual ~EdgeElement();

	void updateData();

	virtual QRectF boundingRect() const;
	QPainterPath shape() const;
	virtual void paint(QPainter* p, QStyleOptionGraphicsItem const *opt, QWidget* w);

	virtual bool initPossibleEdges();
	virtual void initTitles();

	bool isDividable();

	/// Adjust link to make its' ends be placed exactly on corresponding ports
	void adjustLink();

	/// Reconnect, arrange links on linear ports and lay out the link depending on its' type
	void layOut();

	/// Reconnect link to exclude intersections with adjacent nodes
	void reconnectToNearestPorts(bool reconnectSrc = true, bool reconnectDst = true);

	NodeElement *src() const;
	NodeElement *dst() const;
	bool isSrc(NodeElement const *node) const;
	bool isDst(NodeElement const *node) const;
	void setSrc(NodeElement *node);
	void setDst(NodeElement *node);

	/// prepare edge to moving from the linker
	void tuneForLinker();

	QPair<qreal, qreal> portIdOn(NodeElement const *node) const;

	/// @return numeric criteria for sorting links on linear ports
	EdgeArrangeCriteria arrangeCriteria(NodeElement const *node, QLineF const &portLine) const;

	NodeElement* otherSide(NodeElement const *node) const;
	void removeLink(NodeElement const *from);

	QPolygonF line() const;
	void setLine(QPolygonF const &line);

	qreal fromPort() const;
	qreal toPort() const;
	void setFromPort(qreal const fromPort);
	void setToPort(qreal const toPort);

	QStringList fromPortTypes() const;
	QStringList toPortTypes() const;

	void placeStartTo(QPointF const &place);
	void placeEndTo(QPointF const &place);
	void moveConnection(NodeElement *node, qreal const portId);

	/// Resort edges connected to linear ports of adjacent nodes
	void arrangeLinearPorts();

	virtual void connectToPort();

	virtual QList<ContextMenuAction*> contextMenuActions(QPointF const &pos);

	QList<PossibleEdge> getPossibleEdges();

	virtual void setColorRect(bool bl);

	void breakPointHandler(QPointF const &pos);
	bool isBreakPointPressed();
	void breakPointUnpressed();

	void highlight(QColor const color = Qt::red);

	EdgeData& data();

	/// Change link type and redraw it
	void changeShapeType(enums::linkShape::LinkShape const shapeType);

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
	NodeElement *getNodeAt(QPointF const &position, bool isStart);

	/// Determine on which side of a node (top, bottom, right or left) the link's end is placed
	NodeSide defineNodePortSide(bool isStart) const;

	/// Align link's intermediate points to grid
	void alignToGrid();

	/// Proxies QGraphicsItem`s setPos filtering out NaNs
	void setPos(qreal x, qreal y);

	/// Proxies QGraphicsItem`s setPos filtering out NaNs
	void setPos(QPointF const &pos);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual QVariant itemChange(GraphicsItemChange change, QVariant const &value);

	virtual void drawStartArrow(QPainter *painter) const;
	virtual void drawEndArrow(QPainter *painter) const;

	Qt::PenStyle mPenStyle;
	int mPenWidth;
	QColor mPenColor;

private slots:
	void reverse();

private:
	void initLineHandler();
	void updateShapeType();

	int indentReductCoeff();

	/// Set mPortTo to next port.
	void searchNextPort();

	/// Create indent of bounding rect, depending on the rect size.
	QPointF boundingRectIndent(QPointF const &point, NodeSide direction);

	/// Returns true, if the sides adjacent.
	bool isNeighbor(NodeSide const &startSide, NodeSide const &endSide) const;

	/// Returns the next clockwise side.
	NodeSide rotateRight(NodeSide side) const;

	void paintEdge(QPainter *painter, QStyleOptionGraphicsItem const *option, bool drawSavedLine) const;
	void drawArrows(QPainter *painter, bool savedLine) const;
	QPen edgePen(QPainter *painter, QColor color, Qt::PenStyle style, int width) const;
	void setEdgePainter(QPainter *painter, QPen pen, qreal opacity) const;

	NodeElement *innermostChild(QList<QGraphicsItem *> const &items, NodeElement * const element) const;
	void updateLongestPart();

	bool reverseActionIsPossible() const;
	bool canConnect(NodeElement const * const node, bool from) const;
	void reversingReconnectToPorts(NodeElement *newSrc, NodeElement *newDst);

	QList<PossibleEdge> mPossibleEdges;

	NodeElement *mSrc;
	NodeElement *mDst;

	LineFactory *mLineFactory; // Takes ownership
	LineHandler *mHandler; // Takes ownership

	qreal mPortFrom;
	qreal mPortTo;

	enums::linkShape::LinkShape mShapeType;

	int mDragType; // is a number of mLine's point we're trying to drag

	int mLongPart;

	QPolygonF mLine; // holds coordinates of polygon points in coordinate system with center in first point
	QColor mColor;

	ContextMenuAction mReverseAction;
	ContextMenuAction mChangeShapeAction;

	bool mBreakPointPressed;

	EdgeData mData;

	bool mModelUpdateIsCalled;  // flag for the infinite updateData()-s liquidating

	bool mIsLoop; // if line is self-closing (mSrc == mDst && mDst)
};

}
