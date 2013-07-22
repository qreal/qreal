#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsLineItem>
#include <QtCore/QSignalMapper>

#include "../../qrkernel/roles.h"
#include "../umllib/nodeElement.h"
#include "../controller/controller.h"
#include "gestures/mouseMovementManager.h"
#include "copyPaste/clipboardHandler.h"

#include "editorViewMVIface.h"

const int arrowMoveOffset = 5;

namespace qReal {
class EditorViewMViface;
class EditorView;
class MainWindow;

namespace commands
{
class CreateElementCommand;
}
}

class EditorViewScene : public QGraphicsScene
{
	Q_OBJECT

public:
	explicit EditorViewScene(QObject *parent);
	~EditorViewScene();

	void addItem(QGraphicsItem *item);

	void clearScene();

	virtual int launchEdgeMenu(EdgeElement *edge, NodeElement *node, const QPointF &scenePos
			, commands::CreateElementCommand **elementCommand = 0);
	//! @arg shiftToParent vector from (0,0) of container Node to new Element (aka localPos)
	virtual qReal::Id createElement(QString const &
			, QPointF const &scenePos
			, bool searchForParents = true
			, commands::CreateElementCommand **createCommand = 0
			, bool executeImmediately = true
			, QPointF const shiftToParent = QPointF());

	virtual void createElement(QMimeData const *mimeData, QPointF const &scenePos
			, bool searchForParents = true
			, commands::CreateElementCommand **createCommandPointer = 0
			, bool executeImmediately = true);

	// is virtual only to trick linker. is used from plugins and generators and we have no intention of
	// including the scene (with dependencies) there
	virtual Element *getElem(qReal::Id const &id) const;
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

	void highlight(qReal::Id const &graphicalId, bool exclusive = true, QColor const &color = Qt::red);
	void dehighlight(qReal::Id const &graphicalId);
	void dehighlight();

	/// Draws pixmap on scene's foreground (doesn't take ownership!)
	void putOnForeground(QPixmap *pixmap);
	/// Deletes pixmap from scene's foreground
	void deleteFromForeground(QPixmap *pixmap);

	QPointF getMousePos() const;
	static QGraphicsRectItem *getPlaceholder();
	NodeElement *findNewParent(QPointF newParentInnerPoint, NodeElement *node);

	void createSingleElement(Id const &id, QString const &name
			, Element *e, QPointF const &position
			, Id const &parentId, bool isFromLogicalModel
			, commands::CreateElementCommand **createCommandPointer = NULL
			, bool executeImmediately = true);
	void insertElementIntoEdge(qReal::Id const &insertedFirstNodeId
			, qReal::Id const &insertedLastNodeId
			, qReal::Id const &parentId
			, bool isFromLogicalModel
			, QPointF const &scenePos
			, QPointF const &shift
			, QList<NodeElement*> elements
			, commands::AbstractCommand *parentCommand = NULL);

	QList<NodeElement*> getNeibors(NodeElement* node);
	void moveDownFromElem(NodeElement* node, QPointF const &scenePos, QPointF const &direction
			, QPointF const &shift, QList<NodeElement*> elements);

	void reConnectLink(EdgeElement * edgeElem);
	void arrangeNodeLinks(NodeElement* node);

	NodeElement* getNodeById(qReal::Id const &itemId);
	EdgeElement* getEdgeById(qReal::Id const &itemId);

	QList<EdgeElement*> getInEdges(NodeElement* node);
	QList<EdgeElement*> getOutEdges(NodeElement* node);

	void deleteElementFromEdge(qReal::Id const &nodeId, QList<QGraphicsItem*> edgesToDelete);

	void itemSelectUpdate();

	/// update (for a beauty) all edges when tab is opening
	void updateEdgesViaNodes();

	void setTitlesVisible(bool visible);
	void onElementParentChanged(Element *element);

public slots:
	qReal::Id createElement(const QString &type);

	void copy();
	void paste(bool logicalCopy);

	/// selects all elements on the current scene
	void selectAll();

	/// update all links
	void updateEdgeElements();

	void cropToItems();

signals:
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
	void changePropertiesActionTriggered();
	void changeAppearanceActionTriggered();
	void printElementsOfRootDiagram();
	void drawIdealGesture();
	void initMouseMoveManager();
	void createEdge(QString const &id);

	/// Creates an object on a diagram by currently drawn mouse gesture. Stops gesture timer.
	void getObjectByGesture();
	/// Updates repository after the move. Controled by the timer.
	void updateMovedElements();

private:
	void setMVIface(EditorViewMViface *mvIface);

	void getLinkByGesture(NodeElement *parent, NodeElement const &child);
	void drawGesture();
	void deleteGesture();
	void createEdgeMenu(QList<QString> const &ids);

	/// sets sceneRect to (0, 0, 1000, 1000) by adding its corners to the scene
	/// (to keep ability of scene rect to grow automatically)
	void initCorners();
	void setCorners(QPointF const &topLeft, QPointF const &bottomRight);

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

	inline bool isArrow(int key);

	static qreal sign(qreal x);

	void moveSelectedItems(int direction);
	QPointF offsetByDirection(int direction);

	Element *mLastCreatedWithEdge;
	commands::CreateElementCommand *mLastCreatedWithEdgeCommand;

	ClipboardHandler mClipboardHandler;

	bool mRightButtonPressed;
	bool mLeftButtonPressed;
	bool mNeedDrawGrid; // if true, the grid will be shown (as scene's background)

	qreal mWidthOfGrid;
	double mRealIndexGrid;

	NodeElement *mHighlightNode;

	QList<QGraphicsItem *> mGesture;
	/// list of pixmaps to be drawn on scene's foreground
	QList<QPixmap *> mForegroundPixmaps;

	qReal::EditorViewMViface *mMVIface;
	qReal::EditorView *mView;

	qReal::MainWindow *mWindow;
	qReal::Controller *mController;

	QList<QAction *> mContextMenuActions;

	QPointF mCurrentMousePos;
	QPointF mCreatePoint;

	gestures::MouseMovementManager *mMouseMovementManager;

	QSignalMapper *mActionSignalMapper;

	QSet<Element *> mHighlightedElements;
	QTimer *mTimer;

	/** @brief timer for update moved elements without lags */
	QTimer *mTimerForArrowButtons;
	/** @brief shift of the move */
	QPointF mOffset;

	/** @brief Is "true" when we just select items on scene, and "false" when we drag selected items */
	bool mShouldReparentItems;

	QGraphicsRectItem *mTopLeftCorner;
	QGraphicsRectItem *mBottomRightCorner;

	/** @brief list of selected items for additional selection */
	QList<QGraphicsItem* >* mSelectList;

	bool mIsSelectEvent;
	bool mTitlesVisible;

	friend class qReal::EditorViewMViface;
};
