/** @file uml_nodeelement.h
 * 	@brief class for an element object on a diagram
 * */

#pragma once

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtGui/QWidget>
#include <QList>

#include "embeddedLinker.h"
#include "sdfrenderer.h"
#include "uml_element.h"
#include "uml_edgeelement.h"
#include "elementImpl.h"

/** @brief size of a point port */
const int kvadratik = 5;
const int widthLineX = 1400;
const int widthLineY = 1000;

namespace UML {
	class NodeElement : public Element
	{
		Q_OBJECT

	public:
		NodeElement(ElementImpl *impl);
		virtual ~NodeElement();

		virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w, SdfRenderer *portrenderer);
		virtual void paint(QPainter *,  const QStyleOptionGraphicsItem *, QWidget *);

		QRectF boundingRect() const;
		QRectF contentsRect() const;

		virtual void updateData();
		void setGeometry(QRectF const &geom);
		void storeGeometry();
		virtual void setName(QString name);

		const QPointF getPortPos(qreal id) const;
		const QPointF getNearestPort(QPointF location) const;

		qreal getPortId(const QPointF &location) const;

		void addEdge(EdgeElement *edge);

		void delEdge(EdgeElement *edge);

		void setPortsVisible(bool value);

		void hideEmbeddedLinkers();

		virtual bool initPossibleEdges();

		bool getPortStatus();

		bool getHavePortStatus();

                /*double getXHor();
		double getYHor();
		double getXVert();
                double getYVert();*/
                QList<double> getBordersValues();

		bool isLowSide(QPointF& point, double x, double y) const;
		bool isHighSide(QPointF& point, double x, double y) const;
		bool isLeftSide(QPointF& point, double x, double y) const;
		bool isRightSide(QPointF& point, double x, double y) const;
		bool isNoBorderX(QPointF& point, double x, double y) const;
		bool isNoBorderY(QPointF& point, double x, double y) const;

		void resizeChild(QRectF newContents, QRectF oldContents);

		virtual QList<ContextMenuAction*> contextMenuActions();
		void switchOnGrid();
		void switchOffGrid();

		bool isClass();

	private slots:
		void switchGrid(bool isChecked);

	private:
		QList<QGraphicsLineItem*> mLines;
		bool mSwitchGrid;  //if true, the object will be aligned to indexGrid
		ContextMenuAction mSwitchGridAction;
		void delUnusedLines();
		void drawLineX(qreal pointX, qreal myY);
		void drawLineY(qreal pointY, qreal myX);
		bool makeJumpX(qreal deltaX, qreal radiusJump, qreal pointX);
		bool makeJumpY(qreal deltaY, qreal radiusJump, qreal pointY);
		void buildLineX(qreal deltaX, qreal radius, bool doAlways, qreal radiusJump, qreal pointX, qreal correctionX, qreal &myX1, qreal &myX2, qreal myY);
		void buildLineY(qreal deltaY, qreal radius, bool doAlways, qreal radiusJump, qreal pointY, qreal correctionY, qreal &myY1, qreal &myY2, qreal myX);
		qreal recountX1();
		qreal recountX2(qreal myX1);
		qreal recountY1();
		qreal recountY2(qreal myY1);
		void makeGridMovingX(qreal myX, int koef, int indexGrid);
		void makeGridMovingY(qreal myY, int koef, int indexGrid);

		static int const objectMinSize = 10;
		//static int const sizeOfForestalling = 25;//TODO: must be used mElementImpl->sizeOfForestalling

		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

		virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

		void changeFoldState();
		void setLinksVisible(bool);

		bool mPortsVisible;

		QList<QPointF> mPointPorts;
		QList<StatLine> mLinePorts;
		QRectF mContents;

		enum DragState {
			None,
			TopLeft,
			Top,
			TopRight,
			Left,
			Right,
			BottomLeft,
			Bottom,
			BottomRight
		};

		NodeElement *getNodeAt(const QPointF &position);

		void adjustLinks();

		QLineF newTransform(const StatLine& port) const;

		void resize(QRectF newContents);
		void updateByChild(NodeElement* item, bool isItemAddedOrDeleted);
		void updateByNewParent();

		void moveChildren(qreal dx, qreal dy);
		void moveChildren(QPointF const &moving);

		void sortChildren();

		qreal minDistanceFromLinePort(int linePortNumber, const QPointF &location) const;
		qreal distanceFromPointPort(int pointPortNumber, const QPointF &location) const;
		qreal getNearestPointOfLinePort(int linePortNumber, const QPointF &location) const;

		QList<EdgeElement *> mEdgeList;

		DragState mDragState;

		QList<EmbeddedLinker*> embeddedLinkers;
		typedef QPair<QPair<QString,QString>,QPair<bool,QString> > PossibleEdge;
		QSet<PossibleEdge> possibleEdges;
		QSet<QPair<bool,QString> > possibleEdgeTypes;

		bool initEmbeddedLinkers();
		void moveEmbeddedLinkers();

		QTransform mTransform;

		ElementImpl* mElementImpl;

		SdfRenderer *mPortRenderer;
		SdfRenderer *mRenderer;

		bool mIsFolded;
		QRectF mFoldedContents;
		QRectF mCurUnfoldedContents;

		bool mLeftPressed;

		NodeElement* mParentNodeElement;

		QPointF mPos;
		bool inHor;
	};
}
