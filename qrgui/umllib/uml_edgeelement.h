#pragma once

#include <QList>
#include <QPair>

#include "uml_element.h"
#include "../pluginInterface/elementImpl.h"

QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);

namespace UML {
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
			bool reconnectToNearestPorts(bool reconnectSrc, bool reconnectDst);
			bool shouldReconnect() const;
			void arrangeSrcAndDst();
			UML::NodeElement *src() const;
			UML::NodeElement *dst() const;
			bool isSrc(UML::NodeElement const *node) const;
			bool isDst(UML::NodeElement const *node) const;
			qreal portIdOn(UML::NodeElement const *node) const;
			QPointF nextFrom(UML::NodeElement const *node) const;
			QPointF connectionPoint(UML::NodeElement const *node) const;
			UML::NodeElement* otherSide(UML::NodeElement const *node) const;
			void removeLink(UML::NodeElement const *from);
			QPolygonF line() const;

			/** @brief Get position of edge's start point*/
			QPointF from() const;
			/** @brief Get position of edge's end point*/
			QPointF to() const;

			void placeStartTo(QPointF const &place);
			void placeEndTo(QPointF const &place);
			void moveConnection(UML::NodeElement *node, qreal const portId);

			virtual void connectToPort();

			virtual QList<ContextMenuAction*> contextMenuActions();

			QList<PossibleEdge> getPossibleEdges();

			virtual void setColorRect(bool bl);
			void breakPointHandler(QPointF const &pos);
			bool isBreakPointPressed();
			void breakPointUnpressed();
			bool mBreakPointPressed;
		protected:
			virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
			virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
			virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

			virtual void drawStartArrow(QPainter * /**< Объект, осуществляющий отрисовку элементов */) const;
			virtual void drawEndArrow(QPainter * /**< Объект, осуществляющий отрисовку элементов */) const;

			Qt::PenStyle mPenStyle;
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

	};
}

