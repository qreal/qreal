#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "../kernel/definitions.h"
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

		/** @brief set element that handles the object */
		void setMaster(NodeElement *element);
		/** @brief notify that mouse cursor is over the NodeElement */
		void setCovered(bool arg);

		virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w );
		/** @brief moves the object somewhere outside the master element */
		virtual void moveTo(QPointF pos);
		QRectF boundingRect() const;


		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event);
		virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event);
		virtual void mousePressEvent( QGraphicsSceneMouseEvent * event);
	private:
		bool covered;
		/** @brief element that handles this object */
		NodeElement *master;
		/** @brief current edge */
		EdgeElement *mEdge;
		/** @brief bounding rect */
		QRectF mRectangle;
		QRectF mInnerRectangle;
	signals:
		void coveredChanged();
	public slots:
		void changeShowState();
	};
}

