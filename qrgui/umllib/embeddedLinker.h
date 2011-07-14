#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "kernel/roles.h"
#include "edgeElement.h"

class NodeElement;
class EmbeddedLinker : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

	public:
		EmbeddedLinker();
		EmbeddedLinker(QGraphicsItem *parent);
		virtual ~EmbeddedLinker();

		void initTitle();
		void generateColor();

		bool isDirected();
		qReal::Id getEdgeType();
		NodeElement* getMaster();

		void setDirected(const bool directed);
		void setMaster(NodeElement* const master);
		void setEdgeType(qReal::Id const & edgeType);

		void takePosition(int index, int maxIndex);

		virtual QRectF boundingRect() const;
		virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w );

		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);

	private:
		EdgeElement* mEdge;
		NodeElement* master;
		ElementTitle* title;

		float size;
		float indent;
		QColor color;
		QRectF mRectangle;
		QRectF mInnerRectangle;

		bool directed;
		qReal::Id edgeType;
};
