#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "../kernel/roles.h"
#include "uml_edgeelement.h"

namespace UML {
	class NodeElement;
	/** @class EmbeddedLinker
	* 	@brief small round thing near the element. provides alternative way to create links
	 * */
	class EmbeddedLinker : public QObject, public QGraphicsItem
	{
		Q_OBJECT
		Q_INTERFACES(QGraphicsItem)

	public:
		EmbeddedLinker();
		EmbeddedLinker(QGraphicsItem *parent);
		virtual ~EmbeddedLinker();

		QRectF boundingRect() const;

		NodeElement* getMaster();
		void setMaster(NodeElement* const element);

		/** @brief notify that mouse cursor is over the NodeElement */
		void setCovered(bool arg);

		virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w );

		virtual void initTitle();
		virtual void generateColor();
		virtual void setColor(QColor color);

		virtual void setDirected(bool directed);
		virtual void setEdgeType(qReal::Id const & edgeType);
		virtual bool isDirected();

		virtual qReal::Id getEdgeType();

		virtual void takePosition(int index, int maxIndex);

		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event);
		virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event);
		virtual void mousePressEvent( QGraphicsSceneMouseEvent * event);
	private:
		ElementTitle* title;

		bool covered;
		/** @brief element that handles this object */
		NodeElement* master;
		/** @brief current edge */
		EdgeElement* mEdge;
		/** @brief bounding rect */
		QRectF mRectangle;
		QRectF mInnerRectangle;

		float size;
		float indent;

		QColor color;
		bool directed;
		qReal::Id edgeType;
	signals:
		void coveredChanged();
	public slots:
		void changeShowState();
	};
}

