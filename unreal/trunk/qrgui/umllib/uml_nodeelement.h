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
#include "sceneGridHandler.h"

/** @brief size of a point port */
const int kvadratik = 5;
const int widthLineX = 1500;
const int widthLineY = 1100;

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

		bool canHavePorts();

		QList<double> borderValues();

		QList<PossibleEdge> getPossibleEdges();

		bool isLowSide(QPointF& point, double x, double y) const;
		bool isHighSide(QPointF& point, double x, double y) const;
		bool isLeftSide(QPointF& point, double x, double y) const;
		bool isRightSide(QPointF& point, double x, double y) const;
		bool isNoBorderX(QPointF& point, double x, double y) const;
		bool isNoBorderY(QPointF& point, double x, double y) const;

		void resizeChild(QRectF newContents, QRectF oldContents);

		virtual QList<ContextMenuAction*> contextMenuActions();
		void switchAlignment(bool isSwitchedOn);

		virtual void setColorRect(bool bl);

		bool getConnectingState();
		void setConnectingState(bool arg);

		void adjustLinks();

	public slots:
		void switchGrid(bool isChecked);

	private:
		ContextMenuAction mSwitchGridAction;
		void delUnusedLines();
		PossibleEdge toPossibleEdge(const StringPossibleEdge & strPossibleEdge);

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

		QSet<PossibleEdge> possibleEdges;
		QSet<PossibleEdgeType> possibleEdgeTypes;

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
		bool isColorRect;

		bool connecting;

		QList<ContextMenuAction*> mBonusContextMenuActions;

		SceneGridHandler *mGrid;

	};
}
