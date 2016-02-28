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

#include <qrgui/plugins/pluginManager/sdfRenderer.h>
#include <qrgui/models/nodeInfo.h>

#include "qrgui/editor/element.h"
#include "qrgui/editor/edgeElement.h"
#include "qrgui/editor/embedded/linkers/embeddedLinker.h"

#include "qrgui/editor/private/sceneGridHandler.h"
#include "qrgui/editor/private/umlPortHandler.h"
#include "qrgui/editor/private/portHandler.h"


namespace qReal {

class NodeElementType;

namespace gui {
namespace editor {

namespace commands {
class ResizeCommand;
}

/// Represents an instance of some node element on diagram.
/// Node elements represent some entity in model, can be dragged and reshaped by mouse and be connected each to other
/// by edge elements.
class QRGUI_EDITOR_EXPORT NodeElement : public Element
{
	Q_OBJECT

public:
	explicit NodeElement(const NodeElementType &type
			, const Id &id
			, const models::Models &models);

	~NodeElement() override;

	QMap<QString, QVariant> graphicalProperties() const;
	QMap<QString, QVariant> logicalProperties() const;

	void paint(QPainter *p, const QStyleOptionGraphicsItem *opt, QWidget *w) override;

	QRectF boundingRect() const override;

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

	/// Returns descriptor of this node element's type.
	const NodeElementType &nodeType() const;

	/// Collects data about this instance and returns structure describing it.
	NodeInfo data() const;

	virtual bool initPossibleEdges();
	QList<PossibleEdge> getPossibleEdges();

	/// Make ports of specified types visible, hide other ports
	void setPortsVisible(const QStringList &types);

	QList<qreal> borderValues() const;

	//void resizeChild(const QRectF &newContents, const QRectF &oldContents);

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
	Element *getPlaceholderNextElement() const;

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
	void switchGrid(bool isChecked);

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

	void deleteGuides();
	QSet<ElementPair> elementsForPossibleEdge(const StringPossibleEdge &edge);

	void initPortsVisibility();
	void connectSceneEvents();

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event) override;

	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

	void paint(QPainter *p, const QStyleOptionGraphicsItem *opt);
	void drawPorts(QPainter *painter, bool mouseOver);

	/**
	 * Recalculates mHighlightedNode according to current mouse scene position.
	 * @param mouseScenePos Current mouse scene position.
	 */
	void recalculateHighlightedNode(const QPointF &mouseScenePos);
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	void setLinksVisible(bool);

	void updateByChild(NodeElement *item, bool isItemAddedOrDeleted);
	void updateByNewParent();
	void updateBySelection();

	void updateChildrenOrder();

	void initEmbeddedLinkers();

	QRectF diagramRenderingRect() const;

	qReal::commands::AbstractCommand *changeParentCommand(const Id &newParent, const QPointF &position) const;

	const NodeElementType &mType;
	models::Exploser &mExploser;

	ContextMenuAction mSwitchGridAction;

	QMap<QString, bool> mPortsVisibility;

	QRectF mContents;
	QList<EdgeElement *> mEdgeList;

	DragState mDragState;
	QPointF mDragPosition;
	commands::ResizeCommand *mResizeCommand;

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

	QPointF mPos;
	bool mSelectionNeeded;

	bool mConnectionInProgress;

	SceneGridHandler *mGrid;
	PortHandler *mPortHandler;

	QGraphicsRectItem *mPlaceholder;
	NodeElement *mHighlightedNode;

	QImage mRenderedDiagram;
	QTimer mRenderTimer;
};

}
}
}
