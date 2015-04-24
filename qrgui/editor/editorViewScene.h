/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	EditorViewScene(const models::Models &models
			, Controller &controller
			/// @todo: move scene customizer properties to metamodel
			, const SceneCustomizer &customizer
			, const Id &rootId
			, QObject *parent = 0);

	void clearScene();

	virtual int launchEdgeMenu(EdgeElement *edge, NodeElement *node, const QPointF &scenePos
			, bool canBeConnected, commands::CreateElementCommand **elementCommand = 0);
	//! @arg shiftToParent vector from (0,0) of container Node to new Element (aka localPos)
	virtual qReal::Id createElement(const QString &
			, const QPointF &scenePos
			, bool searchForParents = true
			, commands::CreateElementCommand **createCommand = 0
			, bool executeImmediately = true
			, const QPointF &shiftToParent = QPointF()
			, const QString &explosionTargetUuid = QString());

	virtual void createElement(const QMimeData *mimeData, const QPointF &scenePos
			, bool searchForParents = true
			, commands::CreateElementCommand **createCommandPointer = 0
			, bool executeImmediately = true);

	// is virtual only to trick linker. is used from plugins and generators and we have no intention of
	// including the scene (with dependencies) there
	virtual Element *getElem(const qReal::Id &id) const;
	Element *findElemAt(const QPointF &position) const;
	NodeElement *findNodeAt(const QPointF &position) const;

	virtual qReal::Id rootItemId() const;
	/// @todo: remove theese getters
	const models::Models &models() const;
	Controller &controller() const;
	const EditorManagerInterface &editorManager() const;
	const SceneCustomizer &customizer() const;

	/// Produces and returns a widget that shows gestures available for this tab.
	/// Transfers owneship.
	QWidget *gesturesPainterWidget() const;

	void setEnabled(bool enabled);

	void setNeedDrawGrid(bool show);
	qreal realIndexGrid();
	void setRealIndexGrid(qreal newIndexGrid);

	bool canBeContainedBy(const qReal::Id &container, const qReal::Id &candidate) const;

	Element *lastCreatedFromLinker() const;

	/// Removes items selected by user with undo possibility.
	void deleteSelectedItems();

	void highlight(const qReal::Id &graphicalId, bool exclusive = true, const QColor &color = Qt::red);
	void dehighlight(const qReal::Id &graphicalId);
	void dehighlight();

	QPointF getMousePos() const;
	static QGraphicsRectItem *getPlaceholder();
	NodeElement *findNewParent(QPointF newParentInnerPoint, NodeElement *node);

	void createSingleElement(const Id &id, const QString &name
			, bool isNode, const QPointF &position
			, const Id &parentId, bool isFromLogicalModel
			, const Id &explosionTarget = Id()
			, commands::CreateElementCommand **createCommandPointer = nullptr
			, bool executeImmediately = true);

	EdgeElement *edgeForInsertion(const QPointF &scenePos);
	void resolveOverlaps(NodeElement* node, const QPointF &scenePos, const QPointF &shift
			, QMap<qReal::Id, QPointF> &shifting) const;
	void returnElementsToOldPositions(QMap<Id, QPointF> const &shifting) const;

	QList<NodeElement*> getCloseNodes(NodeElement* node) const;

	void reConnectLink(EdgeElement * edgeElem);
	void arrangeNodeLinks(NodeElement* node) const;

	NodeElement* getNodeById(const qReal::Id &itemId) const;
	EdgeElement* getEdgeById(const qReal::Id &itemId) const;

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

	/// Handles deletion of the element from scene.
	void onElementDeleted(Element *element);

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
	void goTo(const Id &id);

	/// Emitted when palette contents could change and thus must be reread.
	void refreshPalette();

	/// Emitted when user requested to change some element`s graphical representation.
	void openShapeEditor(const Id &id
		, const QString &propertyValue
		/// @todo: whan passing it by reference the build on travis fails
		, const EditorManagerInterface *editorManagerProxy
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

	virtual void drawBackground(QPainter *painter, const QRectF &rect);

private slots:
	void createEdge(const QString &id);

	/// Creates an object on a diagram by currently drawn mouse gesture. Stops gesture timer.
	void getObjectByGesture();
	/// Updates repository after the move. Controled by the timer.
	void updateMovedElements();

	void deselectLabels();

private:
	void deleteElements(IdList &idsToDelete);

	void getLinkByGesture(NodeElement *parent, const NodeElement &child);
	void drawGesture();
	void createEdgeMenu(QList<QString> const &ids);

	/// sets sceneRect to (0, 0, 1000, 1000) by adding its corners to the scene
	/// (to keep ability of scene rect to grow automatically)
	void initCorners();
	void setCorners(const QPointF &topLeft, const QPointF &bottomRight);

	void initializeActions();
	void initContextMenu(Element *e, const QPointF &pos);
	bool isEmptyClipboard();

	void disableActions(Element *focusElement);

	inline bool isArrow(int key);

	void moveSelectedItems(int direction);
	bool moveNodes();
	void moveEdges();
	QPointF offsetByDirection(int direction);

	const models::Models &mModels;
	const EditorManagerInterface &mEditorManager;
	Controller &mController;
	const SceneCustomizer &mCustomizer;
	const Id mRootId;

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
