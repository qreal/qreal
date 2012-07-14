
/**
 * @file uml_nodeelement.h
 * @brief class for an element object on a diagram
 */

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
#include "../editorPluginInterface/elementImpl.h"
#include "embedded/linkers/embeddedLinker.h"

#include "sceneGridHandler.h"
#include "umlPortHandler.h"

#include "serializationData.h"

class NodeElement : public Element
{
	Q_OBJECT

public:
	NodeElement(ElementImpl *impl);
	virtual ~NodeElement();

	NodeElement *clone(bool toCursorPos = false, bool searchForParents = true);
	void copyChildren(NodeElement *source);
	void copyEdges(NodeElement *source);
	void copyProperties(NodeElement *source);

	QMap<QString, QVariant> properties();

	virtual void paint(QPainter *p, QStyleOptionGraphicsItem const *opt, QWidget *w, SdfRenderer *portrenderer);
	virtual void paint(QPainter *p, QStyleOptionGraphicsItem const *opt, QWidget *w);

	QRectF boundingRect() const;
	QRectF contentsRect() const;

	virtual void updateData();
	void setGeometry(QRectF const &geom);
	void setPos(QPointF const &pos);
	void setPos(qreal x, qreal y);

	/// Aligning the element to grid
	void alignToGrid();

	void storeGeometry();
	virtual void setName(QString name);

	const QPointF getPortPos(qreal id) const;
	static int portId(qreal id);
	const QPointF getNearestPort(QPointF const &location) const;

	qreal getPortId(QPointF const &location) const;

	void addEdge(EdgeElement *edge);
	void delEdge(EdgeElement *edge);

	NodeData& data();

	virtual bool initPossibleEdges();
	QList<PossibleEdge> getPossibleEdges();

	void setPortsVisible(bool value);

	void hideEmbeddedLinkers();

	bool isPort();
	bool canHavePorts();

	QList<double> borderValues();

	bool checkLowerBorder(QPointF &point, double x, double y) const;
	bool checkUpperBorder(QPointF &point, double x, double y) const;
	bool checkLeftBorder(QPointF &point, double x, double y) const;
	bool checkRightBorder(QPointF &point, double x, double y) const;
	bool checkNoBorderX(QPointF &point, double x, double y) const; // TODO: rename
	bool checkNoBorderY(QPointF &point, double x, double y) const;

	void resizeChild(QRectF newContents, QRectF oldContents);

	virtual QList<ContextMenuAction *> contextMenuActions();
	void switchAlignment(bool isSwitchedOn);
	void showAlignment(bool isChecked);

	virtual void setColorRect(bool bl);

	bool connectionInProgress();
	void setConnectingState(bool arg);

	void adjustLinks();
	void arrangeLinearPorts();
	void arrangeLinks();

	virtual void checkConnectionsToPort();
	virtual void connectLinksToPorts();

	/** @brief Drawing placeholder at the appropriate position (calculated using event data) */
	void drawPlaceholder(QGraphicsRectItem *placeholder, QPointF scenePos);
	void erasePlaceholder(bool);

	/**
	* @brief Returns element that follows placeholder
	* @return element or NULL
	*/
	Element *getPlaceholderNextElement();
	void highlightEdges();

public slots:
	virtual void singleSelectionState(const bool singleSelected);
	virtual void selectionState(const bool selected);
	void switchGrid(bool isChecked);
	NodeElement *copyAndPlaceOnDiagram(QPointF const &offset);

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


	/** @brief Padding that reserves space for title */
	static int const titlePadding = 25;
	/** @brief Space between children inside sorting containers */
	static int const childSpacing = 10;

	void delUnusedLines();
	PossibleEdge toPossibleEdge(const StringPossibleEdge & strPossibleEdge);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event);

	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	virtual QVariant itemChange(GraphicsItemChange change, QVariant const &value);

	void changeFoldState();
	void setLinksVisible(bool);

	NodeElement *getNodeAt(QPointF const &position);

	QLineF newTransform(StatLine const &port) const;
	QPointF newTransform(StatPoint const &port) const;

	void resize(QRectF newContents, QPointF newPos);
	// newPos = mPos
	void resize(QRectF newContents);
	// newContents = mContents
	void resize();

	void updateByChild(NodeElement *item, bool isItemAddedOrDeleted);
	void updateByNewParent();

	void moveChildren(qreal dx, qreal dy);
	void moveChildren(QPointF const &moving);

	void sortChildren();

	qreal minDistanceFromLinePort(int linePortNumber, QPointF const &location) const;
	qreal distanceFromPointPort(int pointPortNumber, QPointF const &location) const;
	qreal getNearestPointOfLinePort(int linePortNumber, QPointF const &location) const;

	void initEmbeddedLinkers();
	void setVisibleEmbeddedLinkers(bool const show);

	void connectTemporaryRemovedLinksToPort(qReal::IdList const &rtemporaryRemovedLinks, QString const &direction);

	ContextMenuAction mSwitchGridAction;
	static int const objectMinSize = 10;
	//static int const sizeOfForestalling = 25; //TODO: must be used mElementImpl->sizeOfForestalling

	bool mPortsVisible;

	QList<NodeElement *> childs;

	QList<StatPoint> mPointPorts;
	QList<StatLine> mLinePorts;
	QRectF mContents;

	QList<EdgeElement *> mEdgeList;

	DragState mDragState;

	QList<EmbeddedLinker *> mEmbeddedLinkers;

	QSet<PossibleEdge> mPossibleEdges;
	QSet<PossibleEdgeType> mPossibleEdgeTypes;

	QTransform mTransform;

	ElementImpl *mElementImpl;

	SdfRenderer *mPortRenderer;
	SdfRenderer *mRenderer;

	bool mIsFolded;
	QRectF mFoldedContents;
	QRectF mCurUnfoldedContents;

	bool mLeftPressed;

	NodeElement *mParentNodeElement;

	QPointF mPos;
	bool mSelectionNeeded;

	bool mConnectionInProgress;

	QList<ContextMenuAction *> mBonusContextMenuActions;

	SceneGridHandler *mGrid;
	UmlPortHandler *mUmlPortHandler;

	QGraphicsRectItem *mPlaceholder;
	NodeElement *mHighlightedNode;

	NodeData mData;
};
