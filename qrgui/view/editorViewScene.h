#pragma once

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QSignalMapper>
#include "../../qrkernel/roles.h"
#include "../umllib/nodeElement.h"
#include "gestures/mouseMovementManager.h"

#include "editorViewMVIface.h"
//#include "editorView.h"
//#include "../mainwindow/mainWindow.h"


//const int indexGrid = 30; // distance between two lines in the grid
const int arrowMoveOffset = 5;

namespace qReal {
class EditorViewMViface;
class EditorView;
class MainWindow;
}

class EditorViewScene : public QGraphicsScene
{
	Q_OBJECT

public:
	explicit EditorViewScene(QObject *parent);
	~EditorViewScene();

	void clearScene();
	virtual int launchEdgeMenu(EdgeElement *edge, NodeElement *node, const QPointF &scenePos);
	virtual qReal::Id createElement(const QString &, QPointF const &scenePos, bool searchForParents = true);
	virtual void createElement(const QMimeData *mimeData, QPointF const &scenePos, bool searchForParents = true);

	// is virtual only to trick linker. is used from plugins and generators and we have no intention of
	// including the scene (with dependencies) there
	virtual Element *getElem(qReal::Id const &id);
	Element *getElemAt(const QPointF &position);

	virtual qReal::Id rootItemId() const;
	void setMainWindow(qReal::MainWindow *mainWindow);
	qReal::MainWindow *mainWindow() const;
	void setEnabled(bool enabled);

	void setNeedDrawGrid(bool show);
	double realIndexGrid();
	void setRealIndexGrid(double newIndexGrid);

	bool canBeContainedBy(qReal::Id const &container, qReal::Id const &candidate) const;
	bool getNeedDrawGrid();

	Element *getLastCreated();

	void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);

	void highlight(qReal::Id const &graphicalId, bool exclusive = true
			, QColor const &color = Qt::red);
	void dehighlight(qReal::Id const &graphicalId);
	void dehighlight();

	/// Draws pixmap on scene's foreground (doesn't take ownership!)
	void putOnForeground(QPixmap *pixmap);
	/// Deletes pixmap from scene's foreground
	void deleteFromForeground(QPixmap *pixmap);

	QPointF getMousePos();
	static QGraphicsRectItem *getPlaceholder();
	NodeElement *findNewParent(QPointF newParentInnerPoint, NodeElement *node);

	void insertNodeIntoEdge(const qReal::Id &insertedNodeId, const qReal::Id &newParent, bool isFromLogicalModel,QPointF const &scenePos);

public slots:
	qReal::Id createElement(const QString &type);
	// TODO: get rid of it here
	void copy();
	void paste(bool logicalCopy);

	/// selects all elements on the current scene
	void selectAll();

signals:
	void elementCreated(qReal::Id const &id);
	void zoomIn();
	void zoomOut();

protected:
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
	void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
	void dropEvent(QGraphicsSceneDragDropEvent *event);

	void keyPressEvent(QKeyEvent *event);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

	virtual void drawForeground(QPainter *painter, QRectF const &rect);
	virtual void drawBackground(QPainter *painter, QRectF const &rect);

private slots:
	void connectActionTriggered();
	void goToActionTriggered();
	void disconnectActionTriggered();
	void addUsageActionTriggered();
	void deleteUsageActionTriggered();
	void printElementsOfRootDiagram();
	void drawIdealGesture();
	void initMouseMoveManager();
	void createEdge(QString const &id);

	/// Creates an object on a diagram by currently drawn mouse gesture. Stops gesture timer.
	void getObjectByGesture();

private:
	void getLinkByGesture(NodeElement *parent, NodeElement const &child);
	void drawGesture();
	void deleteGesture();
	void createEdgeMenu(QList<QString> const &ids);

	void drawGrid(QPainter *painter, const QRectF &rect);
	void redraw();
	void createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const;
	void createGoToSubmenu(QMenu * const goToMenu, QString const &name, qReal::IdList const &ids) const;
	void createAddConnectionMenu(Element const * const element
			, QMenu &contextMenu, QString const &menuName
			, qReal::IdList const &connectableTypes, qReal::IdList const &alreadyConnectedElements
			, qReal::IdList const &connectableDiagrams, const char *slot) const;

	void createDisconnectMenu(Element const * const element
			, QMenu &contextMenu, QString const &menuName
			, qReal::IdList const &outgoingConnections, qReal::IdList const &incomingConnections
			, const char *slot) const;

	void initContextMenu(Element *e, QPointF const &pos);
	bool isEmptyClipboard();

	void disableActions(Element *focusElement);
	void enableActions();

	QList<NodeElement *> getNodesForCopying();
	QList<NodeData> getNodesData(QList<NodeElement *> const &nodes);
	QList<EdgeData> getEdgesData(QList<NodeElement *> const &nodes);

	void addChildren(NodeElement *node, QList<NodeElement *> &nodes);

	void pushDataToClipboard(QList<NodeData> const &nodesData, QList<EdgeData> const &edgesData);
	void pullDataFromClipboard(QList<NodeData> &nodesData, QList<EdgeData> &edgesData);

	QHash<qReal::Id, qReal::Id> pasteNodes(QList<NodeData> &nodesData
			, QPointF const &offset, bool isGraphicalCopy);
	qReal::Id pasteNode(NodeData const &nodeData, bool isGraphicalCopy
			, QHash<qReal::Id, qReal::Id> const &copiedIds, QPointF const &offset);

	NodeElement *pasteGraphicalCopyOfNode(NodeData const &nodeData, QPointF const &newPos);
	NodeElement *pasteNewNode(NodeData const &data, QPointF const &newPos);

	qReal::Id pasteEdge(EdgeData const &edgeData, bool isGraphicalCopy
			, QHash<qReal::Id, qReal::Id> const &copiedIds, QPointF const &offset);
	EdgeElement *pasteGraphicalCopyOfEdge(EdgeData const &edgeData);
	EdgeElement *pasteNewEdge(EdgeData const &edgeData);

	void restoreNode(NodeElement *node, NodeData const &nodeData
			, QHash<qReal::Id, qReal::Id> const &copiedIdsMap, QPointF const &pos);
	void restoreEdge(EdgeElement *edge, EdgeData const &edgeData
			, QHash<qReal::Id, qReal::Id> const &copiedIdsMap, QPointF const &pos);

	QPointF getNewPos(NodeData const &nodeData
			, QHash<qReal::Id, qReal::Id> const &copiedIds, QPointF const &offset);

	inline bool isArrow(int key);

	void moveSelectedItems(int direction);
	QPointF offsetByDirection(int direction);

	Element *mLastCreatedWithEdge;

	bool mRightButtonPressed;
	bool mNeedDrawGrid; // if true, the grid will be shown (as scene's background)

	qreal mWidthOfGrid;
	double mRealIndexGrid;

	NodeElement *mHighlightNode;
	QPointF newElementsPosition;

	QList<QGraphicsItem *> mGesture;
	/// list of pixmaps to be drawn on scene's foreground
	QList<QPixmap *> mForegroundPixmaps;

	qReal::EditorViewMViface *mMVIface;
	qReal::EditorView *mView;

	qReal::MainWindow *mWindow;

	QList<QAction *> mContextMenuActions;

	QPointF mPrevPosition;
	QPointF mCurrentMousePos;
	QGraphicsItem *mPrevParent;

	QPointF mCreatePoint;

	MouseMovementManager *mMouseMovementManager;

	QSignalMapper *mActionSignalMapper;

	QSet<Element *> mHighlightedElements;
	QTimer *mTimer;

	/** @brief Is "true" when we just select items on scene, and "false" when we drag selected items */
	bool mShouldReparentItems;

	friend class qReal::EditorViewMViface;
};
