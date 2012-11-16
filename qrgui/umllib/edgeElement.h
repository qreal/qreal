#pragma once

#include <QList>
#include <QPair>

#include "element.h"
#include "../editorPluginInterface/elementImpl.h"
#include "serializationData.h"

QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);

enum ArrowType { FILLED_ARROW, EMPTY_ARROW, FILLED_RHOMB, EMPTY_RHOMB, NO_ARROW, OPEN_ARROW };

class NodeElement;
/** @class EdgeElement
  * 	@brief class for an edge on a diagram
  * 	*/

class EmbeddedLinker;

class EdgeElement : public Element
{
	Q_OBJECT
public:
	friend class EmbeddedLinker;

	EdgeElement(ElementImpl *impl);
	virtual ~EdgeElement();

	void updateData();

	virtual QRectF boundingRect() const;
	QPainterPath shape() const;
	virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w);

	virtual bool initPossibleEdges();
	virtual void initTitles();

	bool isDividable();
	void adjustLink(bool isDragging = false);

	// use adjustLink() to all links that have with this general master
	void adjustNeighborLinks();
	bool reconnectToNearestPorts(bool reconnectSrc = true, bool reconnectDst = true, bool jumpsOnly = false);
	bool shouldReconnect() const;
	void arrangeSrcAndDst();
	NodeElement *src() const;
	NodeElement *dst() const;
	bool isSrc(NodeElement const *node) const;
	bool isDst(NodeElement const *node) const;
	qreal portIdOn(NodeElement const *node) const;
	QPointF nextFrom(NodeElement const *node) const;
	QPointF connectionPoint(NodeElement const *node) const;
	NodeElement* otherSide(NodeElement const *node) const;
	void removeLink(NodeElement const *from);
	QPolygonF line() const;

	/** @brief Get position of edge's start point*/
	QPointF from() const;
	/** @brief Get position of edge's end point*/
	QPointF to() const;

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

	virtual void deleteFromScene();

	// redrawing of this edgeElement in squarize
	void redrawing(QPointF const &pos);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	virtual QVariant itemChange(GraphicsItemChange change, QVariant const &value);

	virtual void drawStartArrow(QPainter * /**< Объект, осуществляющий отрисовку элементов */) const;
	virtual void drawEndArrow(QPainter * /**< Объект, осуществляющий отрисовку элементов */) const;

	Qt::PenStyle mPenStyle;
	int mPenWidth;
	QColor mPenColor;
	QString mText;
	QString mFromMult, mToMult;
	ArrowType mStartArrowStyle;
	ArrowType mEndArrowStyle;

public slots:
	void setGraphicApi(QPointF const &pos);
	// redraw a new mLine after delPointHandler or deleteSegmentHandler using contextMenu
	void arrangeAndAdjustHandler(QPointF const &pos);

private slots:
	void addPointHandler(QPointF const &pos);
	// add the closest point on edge to the parameter`s point
	void addClosestPointHandler(QPointF const &pos);
	void delPointHandler(QPointF const &pos);
	void squarizeHandler(QPointF const &pos, bool xIsChanged = true);
	void minimizeHandler(QPointF const &pos);
	// delete Segment with nearest with pos ends
	void deleteSegmentHandler(QPointF const &pos);
	// redraw in rectangular good link
	void squarizeAndAdjustHandler(QPointF const &pos);
	// change link's direction
	void reverseHandler(QPointF const &pos);

private:
	// when (mSrc == mDst && mDst && mLine <= 3)
	void createCircleLink();
	// connectToPort for self-closing line (mSrc == mDst && mDst)
	void connectCircleEdge(NodeElement *newMaster);
	// redraw in rectangular good segment by the second squarize method.
	void specialSquarizeSegment(int const /**< The interval's start position in the mLine */);
	// redraw in rectangular good link by the second squarize method
	void specialSquarizeLink();

	QList<PossibleEdge> possibleEdges;

	bool mIsDissectable;
	int getPoint(const QPointF &location);
	NodeElement *getNodeAt(const QPointF &position, bool isStart);
	void updateLongestPart();
	static QRectF getPortRect(QPointF const &point);

	void drawPort(QPainter *painter) const;

	qreal lengthOfSegment(QPointF const &pos1, QPointF const &pos2) const;

	void delCloseLinePoints();

	void delClosePoints();

	bool removeOneLinePoints(int startingPoint);

	void deleteLoops();
	void deleteLoop(int startPos);
	QPointF* haveIntersection(QPointF const &pos1, QPointF const &pos2, QPointF const &pos3, QPointF const &pos4);

	// these methods are called before the push action in the context menu
	bool delPointActionIsPossible(const QPointF &pos);
	bool squarizeActionIsPossible();
	bool addPointActionIsPossible(const QPointF &pos);
	bool delSegmentActionIsPossible(const QPointF &pos);
	bool minimizeActionIsPossible();
	bool reverseActionIsPossible();

	NodeElement *mSrc;
	NodeElement *mDst;

	qreal mPortFrom;
	qreal mPortTo;

	int mDragPoint; // is a number of mLine's point we're trying to drag

	int mLongPart;

	QPolygonF mLine; // holds coordinates of polygon points in coordinate system with center in first point
	QColor mColor;

	ContextMenuAction mAddPointAction;
	ContextMenuAction mDelPointAction;
	ContextMenuAction mSquarizeAction;
	ContextMenuAction mMinimizeAction;
	ContextMenuAction mDelSegmentAction;
	ContextMenuAction mReverseAction;

	bool mChaoticEdition;

	QPolygonF mLastLine;
	QPolygonF mSavedLineForMove;

	bool mBreakPointPressed;

	EdgeData mData;

	bool mModelUpdateIsCalled;

	bool isCircle; // if line is self-closing (mSrc == mDst && mDst)
};

