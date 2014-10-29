#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsLineItem>
#include <QtCore/QSignalMapper>

#include <qrkernel/roles.h>
#include <qrutils/graphicsUtils/gridDrawer.h>
#include <qrgui/mouseGestures/mouseMovementManager.h>

#include "qrgui/editor/editorDeclSpec.h"
#include "qrgui/editor/copyPaste/clipboardHandler.h"
#include "qrgui/editor/private/exploserView.h"

namespace qReal {

const int arrowMoveOffset = 5;

namespace commands {
class CreateElementCommand;
}

class QRGUI_EDITOR_EXPORT EditorViewScene : public QGraphicsScene
{
	Q_OBJECT

public:
	EditorViewScene(models::Models const &models
			, Controller &controller
			/// @todo: move scene customizer properties to metamodel
			, SceneCustomizer const &customizer
			, Id const &rootId
			, QObject *parent = 0);

	void clearScene();

	virtual int launchEdgeMenu(EdgeElement *edge, NodeElement *node, const QPointF &scenePos
			, bool canBeConnected, commands::CreateElementCommand **elementCommand = 0);
	//! @arg shiftToParent vector from (0,0) of container Node to new Element (aka localPos)
	virtual qReal::Id createElement(QString const &
			, QPointF const &scenePos
			, bool searchForParents = true
			, commands::CreateElementCommand **createCommand = 0
			, bool executeImmediately = true
			, QPointF const &shiftToParent = QPointF()
			, QString const &explosionTargetUuid = QString());

	virtual void createElement(QMimeData const *mimeData, QPointF const &scenePos
			, bool searchForParents = true
			, commands::CreateElementCommand **createCommandPointer = 0
			, bool executeImmediately = true);

	// is virtual only to trick linker. is used from plugins and generators and we have no intention of
	// including the scene (with dependencies) there
	virtual Element *getElem(qReal::Id const &id) const;
	Element *findElemAt(QPointF const &position) const;
	NodeElement *findNodeAt(QPointF const &position) const;

	virtual qReal::Id rootItemId() const;
	/// @todo: remove theese getters
	models::Models const &models() const;
	Controller &controller() const;
	EditorManagerInterface const &editorManager() const;
	SceneCustomizer const &customizer() const;

	/// Produces and returns a widget that shows gestures available for this tab.
	/// Transfers owneship.
	QWidget *gesturesPainterWidget() const;

	void setEnabled(bool enabled);

	void setNeedDrawGrid(bool show);
	qreal realIndexGrid();
	void setRealIndexGrid(qreal newIndexGrid);

	bool canBeContainedBy(qReal::Id const &container, qReal::Id const &candidate) const;

	Element *lastCreatedFromLinker() const;

	/// Removes items selected by user with undo possibility.
	void deleteSelectedItems();

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

	/// Returns a reference to action that removes selected elements from the scene.
	QAction &deleteAction();

	/// Returns a list of scene actions that can be added to other views.
	/// Currently those are copy, cut, paste and paste reference actions.
	QList<QAction *> const &editorActions() const;

	/// Enables or diasables all editor actions.
	void setActionsEnabled(bool enabled);

public slots:
	qReal::Id createElement(const QString &type);

	void cut();
	void copy();
	void paste(bool logicalCopy);

	/// selects all elements on the current scene
	void selectAll();

	/// update all links
	void updateEdgeElements();

	void deleteGesture();

	/// Makes same as QGraphicsScene::update. Useful for c++11-styled connections.
	void redraw();

signals:
	void zoomIn();
	void zoomOut();

	/// Emitted when user requested to switch to some element (for example doubled-clicked element with explosion).
	void goTo(Id const &id);

	/// Emitted when palette contents could change and thus must be reread.
	void refreshPalette();

	/// Emitted when user requested to change some element`s graphical representation.
	void openShapeEditor(Id const &id
		, QString const &propertyValue
		/// @todo: whan passing it by reference the build on travis fails
		, EditorManagerInterface const *editorManagerProxy
		, bool useTypedPorts);

protected:
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
	void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
	void dropEvent(QGraphicsSceneDragDropEvent *event);

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

	void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);

	virtual void drawForeground(QPainter *painter, QRectF const &rect);
	virtual void drawBackground(QPainter *painter, QRectF const &rect);

private slots:
	void createEdge(QString const &id);

	/// Creates an object on a diagram by currently drawn mouse gesture. Stops gesture timer.
	void getObjectByGesture();
	/// Updates repository after the move. Controled by the timer.
	void updateMovedElements();

	void deselectLabels();

private:
	void deleteElements(IdList &idsToDelete);

	void getLinkByGesture(NodeElement *parent, NodeElement const &child);
	void drawGesture();
	void createEdgeMenu(QList<QString> const &ids);

	/// sets sceneRect to (0, 0, 1000, 1000) by adding its corners to the scene
	/// (to keep ability of scene rect to grow automatically)
	void initCorners();
	void setCorners(QPointF const &topLeft, QPointF const &bottomRight);

	void initializeActions();
	void initContextMenu(Element *e, QPointF const &pos);
	bool isEmptyClipboard();

	void disableActions(Element *focusElement);

	inline bool isArrow(int key);

	void moveSelectedItems(int direction);
	bool moveNodes();
	void moveEdges();
	QPointF offsetByDirection(int direction);

	models::Models const &mModels;
	EditorManagerInterface const &mEditorManager;
	Controller &mController;
	SceneCustomizer const &mCustomizer;
	Id const mRootId;

	Id mLastCreatedFromLinker;
	commands::CreateElementCommand *mLastCreatedFromLinkerCommand;

	ClipboardHandler mClipboardHandler;

	bool mRightButtonPressed;
	bool mLeftButtonPressed;
	bool mNeedDrawGrid; // if true, the grid will be shown (as scene's background)

	qreal mWidthOfGrid;
	qreal mRealIndexGrid;
	graphicsUtils::GridDrawer mGridDrawer;

	NodeElement *mHighlightNode;

	QList<QGraphicsItem *> mGesture;
	/// list of pixmaps to be drawn on scene's foreground
	QList<QPixmap *> mForegroundPixmaps;

	QList<QAction *> mEditorActions;

	QPointF mCurrentMousePos;
	QPointF mCreatePoint;

	gestures::MouseMovementManager mMouseMovementManager;

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

	QMenu mContextMenu;

	view::details::ExploserView mExploser;
	QAction mActionDeleteFromDiagram;
	QAction mActionCutOnDiagram;
	QAction mActionCopyOnDiagram;
	QAction mActionPasteOnDiagram;
	QAction mActionPasteReference;
};

}
