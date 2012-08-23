
/** @file nodeElement.h
 *  @brief class for an element object on a diagram
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
#include "../editorPluginInterface/elementImpl.h"
#include "embedded/linkers/embeddedLinker.h"

#include "sceneGridHandler.h"
#include "umlPortHandler.h"
#include "portHandler.h"

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
	/// Current value of mContents
	QRectF contentsRect() const;
	/// Folded contents of node
	QRectF foldedContentsRect() const;

	virtual void updateData();
	void setGeometry(QRectF const &geom);
	void setPos(QPointF const &pos);
	void setPos(qreal x, qreal y);

	/// Aligning the element to grid
	void alignToGrid();

	void storeGeometry();
	virtual void setName(QString name);

	QPointF const portPos(qreal id) const;
	QPointF const nearestPort(QPointF const &location) const;
	//TODO: rename method to portNumber
	static int portId(qreal id);
	qreal portId(QPointF const &location) const;

	QList<EdgeElement *> getEdges();
	void addEdge(EdgeElement *edge);
	void delEdge(EdgeElement *edge);

	NodeData& data();

	virtual bool initPossibleEdges();
	QList<PossibleEdge> getPossibleEdges();

	void setPortsVisible(bool value);

	void hideEmbeddedLinkers();

	bool isPort() const;
	bool canHavePorts();

	QList<double> borderValues() const;

	//void resizeChild(QRectF const &newContents, QRectF const &oldContents);

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

	bool isFolded() const;
	QGraphicsRectItem* placeholder() const;

	virtual void deleteFromScene();

	QList<EdgeElement *> const edgeList() const;

	virtual void setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi
			, qReal::models::LogicalModelAssistApi *logicalAssistApi);

public slots:
	virtual void singleSelectionState(bool const singleSelected);
	virtual void selectionState(bool const selected);
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

	/**
	 * Resizes node trying to use newContents as new shape
	 * of node (ignoring newContents position) and to move
	 * node to newPos.
	 * These parameters are corrected by child configuration
	 * in most cases.
	 * @param newContents Recommendation for new shape of node.
	 * @param newPos Recommendation for new position of node.
	 */
	void resize(QRectF newContents, QPointF newPos);

	/**
	 * Calls resize(QRectF newContents, QPointF newPos) with
	 * newPos equals to current position of node.
	 * @param newContents Recommendation for new shape of node.
	 */
	void resize(QRectF newContents);

	/**
	 * Calls resize(QRectF newContents, QPointF newPos) with
	 * newPos equals to current position of node and
	 * newContents equals to current shape (mContents).
	 */
	void resize();

	void disconnectEdges();

	void delUnusedLines();
	PossibleEdge toPossibleEdge(StringPossibleEdge const &strPossibleEdge);

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

	void updateByChild(NodeElement *item, bool isItemAddedOrDeleted);
	void updateByNewParent();

	void initEmbeddedLinkers();
	void setVisibleEmbeddedLinkers(bool const show);

	ContextMenuAction mSwitchGridAction;

	bool mPortsVisible;

	QList<NodeElement *> childs;

	QRectF mContents;
	QList<EdgeElement *> mEdgeList;

	DragState mDragState;
	QPointF mDragPosition;

	QList<EmbeddedLinker *> mEmbeddedLinkers;

	QSet<PossibleEdge> mPossibleEdges;
	QSet<PossibleEdgeType> mPossibleEdgeTypes;

	QTransform mTransform;

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
	PortHandler *mPortHandler;

	QGraphicsRectItem *mPlaceholder;
	NodeElement *mHighlightedNode;

	NodeData mData;
};
