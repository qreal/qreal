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

class EdgeElement : public Element
{
	Q_OBJECT

public:
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

	/// use adjustLink() to all links that have with this general master
	void adjustNeighborLinks();
	bool reconnectToNearestPorts(bool reconnectSrc = true, bool reconnectDst = true, bool jumpsOnly = false);
	bool shouldReconnect() const;
	void arrangeSrcAndDst();
	NodeElement *src() const;
	NodeElement *dst() const;
	bool isSrc(NodeElement const *node) const;
	bool isDst(NodeElement const *node) const;
	void setSrc(NodeElement *node);
	void setDst(NodeElement *node);
	/// prepare edge to moving from the linker
	void tuneForLinker();
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

	/// correct end of mLine for beauty link's drawing, creating the beauty link without node's intersections
	void correctArrow();
	/// correct begin of mLine for beauty link's drawing, creating the beauty link without node's intersections
	void correctInception();

	void setGraphicApiPos();

	bool isLoop();

	void alignToGrid(int const indexGrid);
	qreal alignedCoordinate(qreal coord, int coef, int const indexGrid) const;
	QPointF alignedPoint(QPointF const point, int const indexGrid) const;

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
	QString mText;
	QString mFromMult, mToMult;
	ArrowType mStartArrowStyle;
	ArrowType mEndArrowStyle;

public slots:
	void saveConfiguration(QPointF const &pos);
	/// redraw a new mLine after delPointHandler or deleteSegmentHandler using contextMenu
	void arrangeAndAdjustHandler(QPointF const &pos);

private slots:
	void addPointHandler(QPointF const &pos);
	/// add the closest point on edge to the parameter`s point
	void addClosestPointHandler(QPointF const &pos);
	void delPointHandler(QPointF const &pos);
	void squarizeHandler(QPointF const &pos);
	void minimizeHandler(QPointF const &pos);
	/// delete Segment with nearest with pos ends
	void deleteSegmentHandler(QPointF const &pos);
	// redraw in rectangular good link
	void squarizeAndAdjustHandler(QPointF const &pos);
	/// change link's direction
	void reverseHandler(QPointF const &pos);
	/// mIsVerticalChanging = !mIsVerticalChanging
	void changeSquarizeType(QPointF const &pos);

private:
	enum DragPointType {
		noDrag = -3,
		overPointMax = -2,
		noPort = -1
	};

	enum LineDirection {
		top = -1,
		bottom = 1,
		left = -2,
		right = 2,
		topInsideNode = -3,
		bottomInsideNode = 3,
		leftInsideNode = -4,
		rightInsideNode = 4
	};

	// when (mSrc == mDst && mDst && mLine <= 3)
	void createLoopEdge();
	// connectToPort for self-closing line (mSrc == mDst && mDst)
	void connectLoopEdge(NodeElement *newMaster);
	// need for correcting links at square drawing
	int defineDirection(bool from);

	void paintSavedEdge(QPainter *painter) const;
	void paintChangedEdge(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
	QPen edgePen(QPainter *painter, QColor color, Qt::PenStyle style, int width) const;
	void setEdgePainter(QPainter *painter, QPen pen, qreal opacity) const;

	QList<PossibleEdge> possibleEdges;

	bool mIsDissectable;
	int getPoint(const QPointF &location);
	NodeElement *getNodeAt(const QPointF &position, bool isStart);
	NodeElement *innermostChild(QList<QGraphicsItem *> const &items, NodeElement *element) const;
	void updateLongestPart();
	static QRectF getPortRect(QPointF const &point);

	void drawPort(QPainter *painter) const;
	void drawPorts(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

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
	bool changeSquarizeTypeActionIsPossible();

	void reversingReconnectToPorts(NodeElement *newSrc, NodeElement *newDst);

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
	ContextMenuAction mChangeSquarizeTypeAction;

	void edgeArrowOverlapsNodeCase(int direct);
	void edgeArrowOverlapsNodeUprightCase(int direct);
	void edgeArrowOverlapsNodeHorizontallyCase(int direct);

	void edgeInceptionOverlapsNodeCase(int direct);
	void edgeInceptionOverlapsNodeUprightCase(int direct);
	void edgeInceptionOverlapsNodeHorizontallyCase(int direct);

	bool mChaoticEdition;

	QPolygonF mLastLine;
	bool mLastLineIsLoop;
	QPolygonF mSavedLineForChanges;
	bool mLeftButtonIsPressed;
	QPolygonF mSavedLineForSquarize;
	int mSavesDragPointForSquarize;

	bool mBreakPointPressed;

	EdgeData mData;

	bool mModelUpdateIsCalled;  // flag for the infinite updateData()-s liquidating

	bool mIsLoop; // if line is self-closing (mSrc == mDst && mDst)

	bool mIsVerticalChanging; // for squarize drawing
};
