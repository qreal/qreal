/** @file nodeElement.h
*  @brief class for an element object on a diagram
**/

#pragma once

#include <QtGui/QKeyEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsSceneHoverEvent>
#include <QtWidgets/QWidget>
#include <QtCore/QList>
#include <QtCore/QTimer>

#include <plugins/pluginManager/sdfRenderer.h>
#include <plugins/editorPluginInterface/elementImpl.h>

#include "qrgui/editor/element.h"
#include "qrgui/editor/edgeElement.h"
#include "qrgui/editor/embedded/linkers/embeddedLinker.h"

#include "qrgui/editor/private/sceneGridHandler.h"
#include "qrgui/editor/private/umlPortHandler.h"
#include "qrgui/editor/private/portHandler.h"

#include "editor/serializationData.h"

namespace qReal {

namespace commands {
class ResizeCommand;
}

class QRGUI_EDITOR_EXPORT NodeElement : public Element
{
	Q_OBJECT

public:
	explicit NodeElement(ElementImpl *impl
			, const Id &id
			, qReal::models::GraphicalModelAssistApi &graphicalAssistApi
			, qReal::models::LogicalModelAssistApi &logicalAssistApi
			, qReal::models::Exploser &exploser
			);

	virtual ~NodeElement();

	/**
	 * Makes copy of current NodeElement.
	 * @param toCursorPos Indicates if need to place new element at cursor position.
	 * @param searchForParents Parameter of createElement method in EditorViewScene.
	 * @return Copy of current NodeElement.
	 */
	NodeElement *clone(bool toCursorPos = false, bool searchForParents = true);

	QMap<QString, QVariant> graphicalProperties() const;
	QMap<QString, QVariant> logicalProperties() const;

	/// Clears prerendered images.
	/// @param zoomFactor - current zoom factor to render images.
	void invalidateImagesZoomCache(qreal zoomFactor);

	virtual void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w);

	QRectF boundingRect() const;
	/// Current value of mContents
	QRectF contentsRect() const;
	/// Folded contents of node
	QRectF foldedContentsRect() const;

	virtual void updateData();
	void setGeometry(const QRectF &geom);
	void setPos(const QPointF &pos);
	void setPos(qreal x, qreal y);

	/// Aligning the element to grid
	void alignToGrid();

	bool isContainer() const;

	void storeGeometry();
	virtual void setName(const QString &name, bool withUndoRedo = false);
	//void shift(const QPointF &pos, EdgeElement* called);

	/// Returns port position relative to the top left corner of NodeElement
	/// (position of NodeElement).
	/// @param id Id that position is returned by this method.
	/// @return Port position relative to the top left corner of NodeElement.
	const QPointF portPos(qreal id) const;

	/// Returns a total number of line and point ports on element.
	int numberOfPorts() const;

	/// Returns port ID in terms of described in 'Useful information' in PortHandler class.
	/// @param location For this point port will be seen. Location is assumed to be in LOCAL NodeElement coordinates!
	/// @return Port ID in terms of described in 'Useful information' in PortHandler class.
	qreal portId(const QPointF &location, const QStringList &types) const;

	/// Returns a closest to the given point point on the port (line or point) on this element.
	/// Location is assumed to be in SCENE coordinates! The result is in scene coordinates too.
	QPointF closestPortPoint(const QPointF &location, const QStringList &types) const;

	/// @return List of edges connected to the node
	QList<EdgeElement *> getEdges() const;

	/// Add edge to node's edge list, rearrange linear ports
	void addEdge(EdgeElement *edge);

	/// Remove edge from node's edge list, rearrange linear ports
	void delEdge(EdgeElement *edge);

	NodeData& data();

	virtual bool initPossibleEdges();
	QList<PossibleEdge> getPossibleEdges();

	/// Make ports of specified types visible, hide other ports
	void setPortsVisible(const QStringList &types);

	QList<qreal> borderValues() const;

	//void resizeChild(const QRectF &newContents, const QRectF &oldContents);

	virtual QList<ContextMenuAction *> contextMenuActions(const QPointF &pos);
	void switchAlignment(bool isSwitchedOn);
	void showAlignment(bool isChecked);

	virtual void setColorRect(bool bl);

	bool connectionInProgress();
	void setConnectingState(bool arg);

	void adjustLinks();
	void arrangeLinearPorts();
	void arrangeLinks();

	virtual void checkConnectionsToPort();

	/** @brief Drawing placeholder at the appropriate position (calculated using event data) */
	void drawPlaceholder(QGraphicsRectItem *placeholder, QPointF scenePos);
	void erasePlaceholder(bool);

	/**
	* @brief Returns element that follows placeholder
	* @return element or nullptr
	*/
	Element *getPlaceholderNextElement();

	void changeExpanded();
	bool isExpanded() const;

	bool isFolded() const;
	QGraphicsRectItem* placeholder() const;

	QList<EdgeElement *> const edgeList() const;
	QList<NodeElement *> const childNodes() const;

	void setVisibleEmbeddedLinkers(const bool show);
	void updateShape(const QString &shape) const;

	void changeFoldState();

	void updateLabels();

	/**
	 * Calls resize(QRectF newContents, QPointF newPos) with
	 * newPos equals to current position of node and
	 * newContents equals to current shape (mContents).
	 */
	void resize();

	/// Create resize command and start tracking resize
	void startResize();

	/// Stop tracking resize, execute resize command
	void endResize();

	/**
	 * @brief sortedChildren
	 * @return children of sorting container sorted in correct order
	 */
	IdList sortedChildren() const;

public slots:
	virtual void select(const bool singleSelected);
	virtual void setSelectionState(const bool selected);
	void switchGrid(bool isChecked);
	NodeElement *copyAndPlaceOnDiagram(const QPointF &offset);

private slots:
	void updateNodeEdges();
	void initRenderedDiagram();

private:
	enum DragState {
		None
		, TopLeft
		, Top
		, TopRight
		, Left
		, Right
		, BottomLeft
		, Bottom
		, BottomRight
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
	void resize(const QRectF &newContents, const QPointF &newPos, bool needResizeParent = true);

	/**
	 * Calls resize(QRectF newContents, QPointF newPos) with
	 * newPos equals to current position of node.
	 * @param newContents Recommendation for new shape of node.
	 */
	void resize(const QRectF &newContents);

	void drawLinesForResize(QPainter *painter);
	void drawSeveralLines(QPainter *painter, int dx, int dy);

	void delUnusedLines();
	QSet<ElementPair> elementsForPossibleEdge(const StringPossibleEdge &edge);

	void initPortsVisibility();

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event);

	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	void paint(QPainter *p, const QStyleOptionGraphicsItem *opt);
	void drawPorts(QPainter *painter, bool mouseOver);

	/**
	 * Recalculates mHighlightedNode according to current mouse scene position.
	 * @param mouseScenePos Current mouse scene position.
	 */
	void recalculateHighlightedNode(const QPointF &mouseScenePos);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	void setLinksVisible(bool);

	void updateByChild(NodeElement *item, bool isItemAddedOrDeleted);
	void updateByNewParent();

	void updateChildrenOrder();

	void initEmbeddedLinkers();

	QRectF diagramRenderingRect() const;

	commands::AbstractCommand *changeParentCommand(const Id &newParent, const QPointF &position) const;

	models::Exploser &mExploser;

	ContextMenuAction mSwitchGridAction;

	QMap<QString, bool> mPortsVisibility;

	QRectF mContents;
	QList<EdgeElement *> mEdgeList;

	DragState mDragState;
	QPointF mDragPosition;
	qReal::commands::ResizeCommand *mResizeCommand;

	QList<EmbeddedLinker *> mEmbeddedLinkers;

	QSet<PossibleEdge> mPossibleEdges;
	QSet<PossibleEdgeType> mPossibleEdgeTypes;

	QTransform mTransform;

	SdfRenderer mRenderer;

	bool mIsExpanded;

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
	PortHandler *mPortHandler;

	QGraphicsRectItem *mPlaceholder;
	NodeElement *mHighlightedNode;

	NodeData mData;

	QImage mRenderedDiagram;
	QTimer mRenderTimer;
};

}
