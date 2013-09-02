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

#include "umllib/sdfRenderer.h"
#include "umllib/element.h"
#include "umllib/edgeElement.h"
#include "umllib/embedded/linkers/embeddedLinker.h"
#include "editorPluginInterface/elementImpl.h"

#include "umllib/private/sceneGridHandler.h"
#include "umllib/private/umlPortHandler.h"
#include "umllib/private/portHandler.h"

#include "umllib/serializationData.h"

namespace qReal {

namespace commands {
class ResizeCommand;
}

class NodeElement : public Element
{
	Q_OBJECT

public:
	explicit NodeElement(ElementImpl *impl
			, Id const &id
			, qReal::models::GraphicalModelAssistApi &graphicalAssistApi
			, qReal::models::LogicalModelAssistApi &logicalAssistApi);

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

	bool isContainer() const;

	void storeGeometry();
	virtual void setName(QString const &name, bool withUndoRedo = false);
	//void shift(QPointF const &pos, EdgeElement* called);

	QPointF const portPos(qreal id) const;
	QPointF const nearestPort(QPointF const &location, QStringList const &types) const;
	int numberOfPorts() const;
	static int portNumber(qreal id);
	qreal portId(QPointF const &location, QStringList const &types) const;

	QList<EdgeElement *> getEdges() const;
	void addEdge(EdgeElement *edge);
	void delEdge(EdgeElement *edge);

	NodeData& data();

	virtual bool initPossibleEdges();
	QList<PossibleEdge> getPossibleEdges();

	void setPortsVisible(bool value);

	bool isPort() const;
	bool canHavePorts();

	QList<double> borderValues() const;

	//void resizeChild(QRectF const &newContents, QRectF const &oldContents);

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
	* @return element or NULL
	*/
	Element *getPlaceholderNextElement();

	void highlightEdges();

	void changeExpanded();
	bool isExpanded() const;

	bool isFolded() const;
	QGraphicsRectItem* placeholder() const;

	QList<EdgeElement *> const edgeList() const;
	QList<NodeElement *> const childNodes() const;

	void setVisibleEmbeddedLinkers(bool const show);
	void updateShape(QString const &shape) const;

	void changeFoldState();

	void updateLabels();

	/**
	 * Calls resize(QRectF newContents, QPointF newPos) with
	 * newPos equals to current position of node and
	 * newContents equals to current shape (mContents).
	 */
	void resize();

	void startResize();
	void endResize();

	/**
	 * @brief sortedChildren
	 * @return children of sorting container sorted in correct order
	 */
	IdList sortedChildren() const;

public slots:
	virtual void singleSelectionState(bool const singleSelected);
	virtual void selectionState(bool const selected);
	void switchGrid(bool isChecked);
	NodeElement *copyAndPlaceOnDiagram(QPointF const &offset);

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
	void resize(QRectF const &newContents, QPointF const &newPos, bool needResizeParent = true);

	/**
	 * Calls resize(QRectF newContents, QPointF newPos) with
	 * newPos equals to current position of node.
	 * @param newContents Recommendation for new shape of node.
	 */
	void resize(QRectF const &newContents);

	void drawLinesForResize(QPainter *painter);
	void drawSeveralLines(QPainter *painter, int dx, int dy);

	void delUnusedLines();
	QSet<ElementPair> elementsForPossibleEdge(StringPossibleEdge const &edge);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event);

	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	void paint(QPainter *p, QStyleOptionGraphicsItem const *opt);

	/**
	 * Recalculates mHighlightedNode according to current mouse scene position.
	 * @param mouseScenePos Current mouse scene position.
	 */
	void recalculateHighlightedNode(QPointF const &mouseScenePos);
	virtual QVariant itemChange(GraphicsItemChange change, QVariant const &value);

	void setLinksVisible(bool);

	void updateByChild(NodeElement *item, bool isItemAddedOrDeleted);
	void updateByNewParent();

	void updateChildrenOrder();

	void initEmbeddedLinkers();

	QRectF diagramRenderingRect() const;

	commands::AbstractCommand *changeParentCommand(Id const &newParent, QPointF const &position) const;

	ContextMenuAction mSwitchGridAction;

	bool mPortsVisible;

	QRectF mContents;
	QList<EdgeElement *> mEdgeList;

	DragState mDragState;
	QPointF mDragPosition;
	qReal::commands::ResizeCommand *mResizeCommand;

	QList<EmbeddedLinker *> mEmbeddedLinkers;

	QSet<PossibleEdge> mPossibleEdges;
	QSet<PossibleEdgeType> mPossibleEdgeTypes;

	QTransform mTransform;

	SdfRenderer *mRenderer;

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
