/** @file nodeElement.h
 * 	@brief class for an element object on a diagram
 * */

#pragma once

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtGui/QWidget>
#include <QList>

#include "sdfRenderer.h"
#include "element.h"
#include "edgeElement.h"
#include "embedded/linkers/embeddedLinker.h"
#include "../pluginInterface/elementImpl.h"

#include "sceneGridHandler.h"
#include "umlPortHandler.h"

const int widthLineX = 1500;
const int widthLineY = 1100;

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

	static int portId(qreal id);
	qreal getPortId(const QPointF &location) const;

	void addEdge(EdgeElement *edge);
	void delEdge(EdgeElement *edge);

	virtual bool initPossibleEdges();
	QList<PossibleEdge> getPossibleEdges();

	void setPortsVisible(bool value);

	void hideEmbeddedLinkers();

	bool isPort();
	bool canHavePorts();

	QList<double> borderValues();

	bool checkLowerBorder(QPointF& point, double x, double y) const;
	bool checkUpperBorder(QPointF& point, double x, double y) const;
	bool checkLeftBorder(QPointF& point, double x, double y) const;
	bool checkRightBorder(QPointF& point, double x, double y) const;
	bool checkNoBorderX(QPointF& point, double x, double y) const; // TODO: rename
	bool checkNoBorderY(QPointF& point, double x, double y) const;

	void resizeChild(QRectF newContents, QRectF oldContents);

	virtual QList<ContextMenuAction*> contextMenuActions();
	void switchAlignment(bool isSwitchedOn);
	void showAlignment(bool isChecked);

	virtual void setColorRect(bool bl);

	bool connectionInProgress();
	void setConnectingState(bool arg);

	void adjustLinks();
	void arrangeLinks();

	virtual void checkConnectionsToPort();

	/** @brief Drawing placeholder at the appropriate position (calculated using event data) */
	void drawPlaceholder(QGraphicsRectItem *placeholder, QGraphicsSceneDragDropEvent *event);
	void erasePlaceholder(bool);

	/** @brief Returning element that follows placeholder
	*   @return element or NULL
	* */
	Element *getPlaceholderNextElement();

public slots:
	virtual void singleSelectionState(const bool singleSelected);
	virtual void selectionState(const bool selected);
	void switchGrid(bool isChecked);

private:
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

	void delUnusedLines();
	void arrangeLinksRecursively(QSet<NodeElement*>& toArrange, QSet<NodeElement*>& arranged);
	PossibleEdge toPossibleEdge(const StringPossibleEdge & strPossibleEdge);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	void changeFoldState();
	void setLinksVisible(bool);

	NodeElement *getNodeAt(const QPointF &position);

	QLineF newTransform(const StatLine& port) const;
	QPointF newTransform(const StatPoint& port) const;

	void resize(QRectF newContents);
	void updateByChild(NodeElement* item, bool isItemAddedOrDeleted);
	void updateByNewParent();

	void moveChildren(qreal dx, qreal dy);
	void moveChildren(QPointF const &moving);

	void sortChildren();

	qreal minDistanceFromLinePort(int linePortNumber, const QPointF &location) const;
	qreal distanceFromPointPort(int pointPortNumber, const QPointF &location) const;
	qreal getNearestPointOfLinePort(int linePortNumber, const QPointF &location) const;

	void initEmbeddedLinkers();
	void setVisibleEmbeddedLinkers(const bool show);

	void connectTemporaryRemovedLinksToPort(qReal::IdList const &rtemporaryRemovedLinks, QString const &direction);

	ContextMenuAction mSwitchGridAction;
	static int const objectMinSize = 10;
	//static int const sizeOfForestalling = 25;//TODO: must be used mElementImpl->sizeOfForestalling
	QGraphicsRectItem *mPlaceholder;

	bool mPortsVisible;

	QList<NodeElement*> childs;

	QList<StatPoint> mPointPorts;
	QList<StatLine> mLinePorts;
	QRectF mContents;

	QList<EdgeElement *> mEdgeList;

	DragState mDragState;

	QList<EmbeddedLinker*> mEmbeddedLinkers;

	QSet<PossibleEdge> mPossibleEdges;
	QSet<PossibleEdgeType> mPossibleEdgeTypes;

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
	bool mSelectionNeeded;

	bool mConnectionInProgress;

	QList<ContextMenuAction*> mBonusContextMenuActions;

	SceneGridHandler *mGrid;
	UmlPortHandler *mUmlPortHandler;

};

