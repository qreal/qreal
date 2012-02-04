#pragma once

#include <QList>
#include <QPair>

#include "element.h"
#include "../editorPluginInterface/elementImpl.h"
#include "../../qrkernel/ids.h"

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

	void adjustLink();
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

	virtual QList<ContextMenuAction*> contextMenuActions();

	QList<PossibleEdge> getPossibleEdges();

	virtual void setColorRect(bool bl);
	void breakPointHandler(QPointF const &pos);
	bool isBreakPointPressed();
	void breakPointUnpressed();

	void disconnectFromNode(qReal::Id const &nodeLogicalId);//qwerty

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	virtual void drawStartArrow(QPainter * /**< Объект, осуществляющий отрисовку элементов */) const;
	virtual void drawEndArrow(QPainter * /**< Объект, осуществляющий отрисовку элементов */) const;

	Qt::PenStyle mPenStyle;
	int mPenWidth;
	QColor mPenColor;
	QString mText;
	QString mFromMult, mToMult;
	ArrowType mStartArrowStyle;
	ArrowType mEndArrowStyle;

private slots:
	void addPointHandler(QPointF const &pos);
	void delPointHandler(QPointF const &pos);
	void squarizeHandler(QPointF const &pos);
	void minimizeHandler(QPointF const &pos);
private:

	QList<PossibleEdge> possibleEdges;

	int getPoint(const QPointF &location);
	NodeElement *getNodeAt(const QPointF &position);
	void updateLongestPart();
	static QRectF getPortRect(QPointF const &point);

	void drawPort(QPainter *painter) const;

	void removeUnneededPoints(int startingPoint);

	void deleteUnneededPoints();

	NodeElement *mSrc;
	NodeElement *mDst;

	qreal mPortFrom;
	qreal mPortTo;

	int mDragPoint; // is a number of mLine's point we're trying to drag

	int mLongPart;

	QPolygonF mLine; // holds coordinates of polygon points in coordinate system with center in first point
	QColor mColor;

	NodeElement *mBeginning;
	NodeElement *mEnding;

	ContextMenuAction mAddPointAction;
	ContextMenuAction mDelPointAction;
	ContextMenuAction mSquarizeAction;
	ContextMenuAction mMinimizeAction;

	bool mChaoticEdition;

	ElementImpl *mElementImpl;

	QPointF mLastPos;
	QPolygonF mLastLine;
	int mLastDragPoint;

	bool mBreakPointPressed;

};

