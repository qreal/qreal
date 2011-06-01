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
			bool reconnectToNearestPorts(bool reconnectSrc = true, bool reconnectDst = true, bool jumpsOnly = false);
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
		private:

			QList<PossibleEdge> possibleEdges;

			int getPoint(const QPointF &location);
			NodeElement *getNodeAt(const QPointF &position);
			void updateLongestPart();
			static QRectF getPortRect(QPointF const &point);

			void drawPort(QPainter *painter) const;

			NodeElement *mSrc;
			NodeElement *mDst;

			qreal mPortFrom;
			qreal mPortTo;
			int mDragState;

			int mLongPart;

			QPolygonF mLine;
			QColor mColor;

			NodeElement *mBeginning;
			NodeElement *mEnding;

			ContextMenuAction mAddPointAction;
			ContextMenuAction mDelPointAction;
			ContextMenuAction mSquarizeAction;

			bool mChaoticEdition;

			ElementImpl *mElementImpl;
	};
}

