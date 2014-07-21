#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsLineItem>
#include <QtCore/QSignalMapper>

#include <qrkernel/roles.h>
#include <qrutils/graphicsUtils/gridDrawer.h>
#include "umllib/nodeElement.h"
#include "controller/controller.h"
#include "view/gestures/mouseMovementManager.h"
#include "view/copyPaste/clipboardHandler.h"

#include "view/private/editorViewMVIface.h"
#include "view/private/exploserView.h"

namespace qReal {

const int arrowMoveOffset = 5;

class EditorViewMViface;
class EditorView;
class MainWindow;

namespace commands {
class CreateElementCommand;
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
			, bool canBeConnected, commands::CreateElementCommand **elementCommand = 0);
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
	Element *getElemAt(const QPointF &position) const;

	virtual qReal::Id rootItemId() const;
	void setMainWindow(qReal::MainWindow *mainWindow);
	qReal::MainWindow *mainWindow() const;
	void setEnabled(bool enabled);

	void setNeedDrawGrid(bool show);
	double realIndexGrid();
	void setRealIndexGrid(double newIndexGrid);

	bool canBeContainedBy(qReal::Id const &container, qReal::Id const &candidate) const;
	bool getNeedDrawGrid();

	Element *lastCreatedFromLinker() const;

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
			, bool isNode, QPointF const &position
			, Id const &parentId, bool isFromLogicalModel
			, Id const &explosionTarget = Id()
			, commands::CreateElementCommand **createCommandPointer = NULL
			, bool executeImmediately = true);

	EdgeElement *edgeForInsertion(QPointF const &scenePos);
	void resolveOverlaps(NodeElement* node, QPointF const &scenePos, QPointF const &shift
			, QMap<qReal::Id, QPointF> &shifting) const;
	void returnElementsToOldPositions(QMap<Id, QPointF> const &shifting) const;

	QList<NodeElement*> getCloseNodes(NodeElement* node) const;

	void reConnectLink(EdgeElement * edgeElem);
	void arrangeNodeLinks(NodeElement* node) const;

	NodeElement* getNodeById(qReal::Id const &itemId) const;
	EdgeElement* getEdgeById(qReal::Id const &itemId) const;

	void itemSelectUpdate();

	/// update (for a beauty) all edges when tab is opening
	void initNodes();

	void setTitlesVisible(bool visible);
	void onElementParentChanged(Element *element);

public slots:
	qReal::Id createElement(const QString &type);

	void cut();
	void copy();
	void paste(bool logicalCopy);

	/// selects all elements on the current scene
	void selectAll();

	/// update all links
	void updateEdgeElements();

	void cropToItems();

	void deleteGesture();

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
	void printElementsOfRootDiagram();
	void drawIdealGesture();
	void initMouseMoveManager();
	void createEdge(QString const &id);

	/// Creates an object on a diagram by currently drawn mouse gesture. Stops gesture timer.
	void getObjectByGesture();
	/// Updates repository after the move. Controled by the timer.
	void updateMovedElements();

	void deselectLabels();

private:
	void setMVIface(EditorViewMViface *mvIface);

	void getLinkByGesture(NodeElement *parent, NodeElement const &child);
	void drawGesture();
	void createEdgeMenu(QList<QString> const &ids);

	/// sets sceneRect to (0, 0, 1000, 1000) by adding its corners to the scene
	/// (to keep ability of scene rect to grow automatically)
	void initCorners();
	void setCorners(QPointF const &topLeft, QPointF const &bottomRight);

	void redraw();

	void initContextMenu(Element *e, QPointF const &pos);
	bool isEmptyClipboard();

	void disableActions(Element *focusElement);
	void enableActions();

	inline bool isArrow(int key);

	void moveSelectedItems(int direction);
	bool moveNodes();
	void moveEdges();
	QPointF offsetByDirection(int direction);

	Id mLastCreatedFromLinker;
	commands::CreateElementCommand *mLastCreatedFromLinkerCommand;

	ClipboardHandler mClipboardHandler;

	bool mRightButtonPressed;
	bool mLeftButtonPressed;
	bool mNeedDrawGrid; // if true, the grid will be shown (as scene's background)

	qreal mWidthOfGrid;
	double mRealIndexGrid;
	graphicsUtils::GridDrawer mGridDrawer;

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
	QList<QGraphicsItem* > mSelectList;

	bool mIsSelectEvent;
	bool mTitlesVisible;

	QMenu mContextMenu;

	view::details::ExploserView *mExploser; // Takes ownership

	friend class qReal::EditorViewMViface;
};

}
