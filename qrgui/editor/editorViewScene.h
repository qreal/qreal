/* Copyright 2007-2016 QReal Research Group
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
#include <QtCore/QScopedPointer>

#include <qrkernel/roles.h>
#include <qrutils/graphicsUtils/gridDrawer.h>
#include <qrgui/mouseGestures/mouseMovementManagerInterface.h>

#include "qrgui/editor/editorDeclSpec.h"
#include "qrgui/editor/private/clipboardHandler.h"
#include "qrgui/editor/private/exploserView.h"

namespace qReal {

namespace commands {
class CreateElementsCommand;
}

namespace gui {
namespace editor {

const int arrowMoveOffset = 5;

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
			, bool canBeConnected, qReal::commands::CreateElementsCommand **elementCommand = 0);

	//! @arg shiftToParent vector from (0,0) of container Node to new Element (aka localPos)
	virtual Id createElement(const QString &idString
			, const QPointF &scenePos
			, qReal::commands::CreateElementsCommand **createCommandPointer = 0
			, bool executeImmediately = true);

	virtual void createElement(const QMimeData *mimeData, const QPointF &scenePos
			, qReal::commands::CreateElementsCommand **createCommandPointer = 0
			, bool executeImmediately = true);

	// is virtual only to trick linker. is used from plugins and generators and we have no intention of
	// including the scene (with dependencies) there
	virtual Element *getElem(const Id &id) const;
	Element *findElemAt(const QPointF &position) const;
	NodeElement *findNodeAt(const QPointF &position) const;

	virtual Id rootItemId() const;
	/// @todo: remove theese getters
	const models::Models &models() const;
	Controller &controller() const;
	const EditorManagerInterface &editorManager() const;
	const qReal::gui::editor::SceneCustomizer &customizer() const;

	/// Produces and returns a widget that shows gestures available for this tab.
	/// Transfers owneship.
	QWidget *gesturesPainterWidget() const;

	void setEnabled(bool enabled);

	void setNeedDrawGrid(bool show);
	qreal realIndexGrid();
	void setRealIndexGrid(qreal newIndexGrid);

	bool canBeContainedBy(const Id &container, const Id &candidate) const;

	Element *lastCreatedFromLinker() const;

	/// Removes items selected by user with undo possibility.
	void deleteSelectedItems();

	void highlight(const Id &graphicalId, bool exclusive = true, const QColor &color = Qt::red);
	void dehighlight(const Id &graphicalId);
	void dehighlight();

	static QGraphicsRectItem *getPlaceholder();
	NodeElement *findNewParent(QPointF newParentInnerPoint, NodeElement *node);

	void createSingleElement(const ElementInfo &element
			, qReal::commands::CreateElementsCommand **createCommandPointer = nullptr
			, bool executeImmediately = true);

	EdgeElement *edgeForInsertion(const QPointF &scenePos);

	void resolveOverlaps(NodeElement* node, const QPointF &scenePos, const QPointF &shift
			, QMap<Id, QPointF> &shifting) const;

	void returnElementsToOldPositions(QMap<Id, QPointF> const &shifting) const;

	QList<NodeElement*> getCloseNodes(NodeElement* node) const;

	void reConnectLink(EdgeElement * edgeElem);
	void arrangeNodeLinks(NodeElement* node) const;

	NodeElement* getNodeById(const Id &itemId) const;
	EdgeElement* getEdgeById(const Id &itemId) const;

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

	/// Enable or Disable mousegestures
	void enableMouseGestures(bool enabled);

public slots:
	Id createElement(const QString &type);

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
	void createEdge(const Id &id);

	/// Creates an object on a diagram by currently drawn mouse gesture. Stops gesture timer.
	void getObjectByGesture();
	/// Updates repository after the move. Controled by the timer.
	void updateMovedElements();

	void deselectLabels();

private:
	void deleteElements(const IdList &idsToDelete);

	void getLinkByGesture(NodeElement *parent, const NodeElement &child);
	void drawGesture();
	void createEdgeMenu(const QList<Id> &ids);

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

	void createElement(const ElementInfo &elementInfo
			, const QPointF &scenePos
			, qReal::commands::CreateElementsCommand **createCommandPointer
			, bool executeImmediately);

	const models::Models &mModels;
	const EditorManagerInterface &mEditorManager;
	Controller &mController;
	const SceneCustomizer &mCustomizer;
	const Id mRootId;

	Id mLastCreatedFromLinker;

	/// Does not have ownership.
	qReal::commands::CreateElementsCommand *mLastCreatedFromLinkerCommand;

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

	QPointF mCreatePoint;

	QScopedPointer<gestures::MouseMovementManagerInterface> mMouseMovementManager;

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

	bool mMouseGesturesEnabled;

	QMenu mContextMenu;

	view::details::ExploserView mExploser;
	QAction mActionDeleteFromDiagram;
	QAction mActionCutOnDiagram;
	QAction mActionCopyOnDiagram;
	QAction mActionPasteOnDiagram;
	QAction mActionPasteReference;
};

}
}
}
