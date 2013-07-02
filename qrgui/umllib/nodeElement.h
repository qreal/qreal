/** @file nodeElement.h
 *  @brief class for an element object on a diagram
 * */

#pragma once

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QtWidgets/QWidget>
#include <QList>

#include "sdfRenderer.h"
#include "element.h"
#include "edgeElement.h"
#include "embedded/linkers/embeddedLinker.h"
#include "../editorPluginInterface/elementImpl.h"
#include "embedded/linkers/embeddedLinker.h"

#include "private/sceneGridHandler.h"
#include "private/umlPortHandler.h"
#include "private/portHandler.h"

#include "serializationData.h"

namespace qReal
{
namespace commands
{
class ResizeCommand;
}
}

class NodeElement : public Element
{
	Q_OBJECT

public:
	explicit NodeElement(ElementImpl *impl);
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

	bool isContainer() const;

	void storeGeometry();
	virtual void setName(QString name);
	//void shift(QPointF const &pos, EdgeElement* called);

	QPointF const portPos(qreal id) const;
	QPointF const nearestPort(QPointF const &location) const;
	static int portNumber(qreal id);
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

	virtual QList<ContextMenuAction *> contextMenuActions(const QPointF &pos);
	void switchAlignment(bool isSwitchedOn);
	void showAlignment(bool isChecked);

	virtual void setColorRect(bool bl);

	bool connectionInProgress();
	void setConnectingState(bool arg);

	void adjustLinks(bool isDragging = false);
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
	QList<NodeElement *> const childNodes() const;

	virtual void setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi
			, qReal::models::LogicalModelAssistApi *logicalAssistApi);

	void setVisibleEmbeddedLinkers(bool const show);
	void updateShape(QString const &shape) const;

	void changeFoldState();

public slots:
	virtual void singleSelectionState(bool const singleSelected);
	virtual void selectionState(bool const selected);
	void switchGrid(bool isChecked);
	NodeElement *copyAndPlaceOnDiagram(QPointF const &offset);

private slots:
	void updateNodeEdges();

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
	void resize(QRectF const &newContents, QPointF const &newPos);

	/**
	 * Calls resize(QRectF newContents, QPointF newPos) with
	 * newPos equals to current position of node.
	 * @param newContents Recommendation for new shape of node.
	 */
	void resize(QRectF const &newContents);

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

	/**
	 * Recalculates mHighlightedNode according to current mouse scene position.
	 * @param mouseScenePos Current mouse scene position.
	 */
	void recalculateHighlightedNode(QPointF const &mouseScenePos);
	virtual QVariant itemChange(GraphicsItemChange change, QVariant const &value);

	void setLinksVisible(bool);

	NodeElement *getNodeAt(QPointF const &position);

	void updateByChild(NodeElement *item, bool isItemAddedOrDeleted);
	void updateByNewParent();

	void initEmbeddedLinkers();

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

	int mTimeOfUpdate;
	QTimer *mTimer;
};
