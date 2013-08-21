#pragma once

#include <QtCore/QList>
#include <QtCore/QPair>

#include "element.h"
#include "../editorPluginInterface/elementImpl.h"
#include "serializationData.h"

namespace qReal {

//QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);

namespace enums {
namespace arrowTypeEnum {
enum ArrowType
{
	  filledArrow
	, emptyArrow
	, filledRhomb
	, emptyRhomb
	, noArrow
	, openArrow
	, crossedLine
	, emptyCircle
};
}
}

class NodeElement;
class LineHandler;

/** @class EdgeElement
* @brief class for an edge on a diagram
*/

class EdgeElement : public Element
{
	Q_OBJECT

public:
	enum DragType {
		wholeEdge = -3,
		overPointMax = -2,
		noPort = -1
	};

	enum NodeSide {
		left,
		top,
		right,
		bottom
	};

	EdgeElement(ElementImpl *impl
			, Id const &id
			, qReal::models::GraphicalModelAssistApi &graphicalAssistApi
			, qReal::models::LogicalModelAssistApi &logicalAssistApi);

	virtual ~EdgeElement();

	void updateData();

	virtual QRectF boundingRect() const;
	QPainterPath shape() const;
	virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w);

	virtual bool initPossibleEdges();
	virtual void initTitles();

	bool isDividable();
	void adjustLink();

	void reconnectToNearestPorts(bool reconnectSrc = true, bool reconnectDst = true);
	void arrangeSrcAndDst();

	NodeElement *src() const;
	NodeElement *dst() const;
	bool isSrc(NodeElement const *node) const;
	bool isDst(NodeElement const *node) const;
	void setSrc(NodeElement *node);
	void setDst(NodeElement *node);

	/// prepare edge to moving from the linker
	void tuneForLinker();
	QPair<qreal, qreal> portIdOn(NodeElement const *node) const;
	QPointF nextFrom(NodeElement const *node) const;
	QPointF connectionPoint(NodeElement const *node) const;
	NodeElement* otherSide(NodeElement const *node) const;
	void removeLink(NodeElement const *from);

	QPolygonF line() const;
	void setLine(QPolygonF const &line);

	qreal fromPort() const;
	qreal toPort() const;
	void setFromPort(qreal const &fromPort);
	void setToPort(qreal const &toPort);

	QStringList fromPortTypes() const;
	QStringList toPortTypes() const;

	void placeStartTo(QPointF const &place);
	void placeEndTo(QPointF const &place);
	void moveConnection(NodeElement *node, qreal const portId);

	virtual void connectToPort();

	virtual QList<ContextMenuAction*> contextMenuActions(const QPointF &pos);

	QList<PossibleEdge> getPossibleEdges();

	virtual void setColorRect(bool bl);
	void breakPointHandler(QPointF const &pos);
	bool isBreakPointPressed();
	void breakPointUnpressed();

	void highlight(QColor const color = Qt::red);

	EdgeData& data();

	void redrawing(QPointF const &pos);

	void setGraphicApiPos();

	bool isLoop();
	void delCloseLinePoints();
	/// Change line, if (mSrc && (mSrc == mDst)).
	void createLoopEdge();
	/// connectToPort for self-closing line (mSrc && (mSrc == mDst)).
	void connectLoopEdge(NodeElement *newMaster);
	void deleteLoops();
	void layOut();
	NodeElement *getNodeAt(const QPointF &position, bool isStart);
	NodeSide defineNodePortSide(bool isStart);

	void alignToGrid();

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual QVariant itemChange(GraphicsItemChange change, QVariant const &value);

	virtual void drawStartArrow(QPainter * /**< Объект, осуществляющий отрисовку элементов */) const;
	virtual void drawEndArrow(QPainter * /**< Объект, осуществляющий отрисовку элементов */) const;

	Qt::PenStyle mPenStyle;
	int mPenWidth;
	QColor mPenColor;

public slots:
	void saveConfiguration();
	/// redraw a new mLine after delPointHandler or deleteSegmentHandler using contextMenu
	void arrangeAndAdjustHandler(QPointF const &pos);

private slots:
	void delPointHandler(QPointF const &pos);
	void minimizeHandler(QPointF const &pos);
	/// delete Segment with nearest with pos ends
	void deleteSegmentHandler(QPointF const &pos);

	/// change link's direction
	void reverseHandler(QPointF const &pos);

private:
	void initLineHandler();

	int indentReductCoeff();
	/// Set mPortTo to next port.
	void searchNextPort();

	/// Create indent of bounding rect, depending on the rect size.
	QPointF boundingRectIndent(QPointF const &point, NodeSide direction);
	/// Returns true, if the sides adjacent.
	bool isNeighbor(const NodeSide &startSide, const NodeSide &endSide) const ;
	/// Returns the next clockwise side.
	NodeSide rotateRight(NodeSide side) const;

	void paintEdge(QPainter *painter, QStyleOptionGraphicsItem const *option, bool drawSavedLine) const;
	void drawArrows(QPainter *painter, bool savedLine) const;
	QPen edgePen(QPainter *painter, QColor color, Qt::PenStyle style, int width) const;
	void setEdgePainter(QPainter *painter, QPen pen, qreal opacity) const;

	NodeElement *innermostChild(QList<QGraphicsItem *> const &items, NodeElement *element) const;
	void updateLongestPart();
	static QRectF getPortRect(QPointF const &point);

	void delClosePoints();

	int getPoint(const QPointF &location);

	bool removeOneLinePoints(int startingPoint);

	void deleteLoop(int startPos);
	QPointF* haveIntersection(QPointF const &pos1, QPointF const &pos2, QPointF const &pos3, QPointF const &pos4);

	// these methods are called before the push action in the context menu
	bool delPointActionIsPossible(const QPointF &pos);
	bool delSegmentActionIsPossible(const QPointF &pos);
	bool minimizeActionIsPossible();
	bool reverseActionIsPossible();

	void reversingReconnectToPorts(NodeElement *newSrc, NodeElement *newDst);

	QList<PossibleEdge> mPossibleEdges;

	NodeElement *mSrc;
	NodeElement *mDst;

	LineHandler *mHandler;

	qreal mPortFrom;
	qreal mPortTo;

	int mDragType; // is a number of mLine's point we're trying to drag

	int mLongPart;

	QPolygonF mLine; // holds coordinates of polygon points in coordinate system with center in first point
	QColor mColor;

	ContextMenuAction mDelPointAction;
	ContextMenuAction mMinimizeAction;
	ContextMenuAction mDelSegmentAction;
	ContextMenuAction mReverseAction;

	bool mBreakPointPressed;

	EdgeData mData;

	bool mModelUpdateIsCalled;  // flag for the infinite updateData()-s liquidating

	bool mIsLoop; // if line is self-closing (mSrc == mDst && mDst)
};

}
