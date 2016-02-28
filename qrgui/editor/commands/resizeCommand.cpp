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

#include "editor/commands/resizeCommand.h"

using namespace qReal::gui::editor::commands;

ResizeCommand::ResizeCommand(const EditorViewScene *scene, const Id &id)
	: NodeElementCommand(scene, id)
{
}

ResizeCommand::ResizeCommand(const EditorView *view, const Id &id)
	: NodeElementCommand(&view->editorViewScene(), id)
{
}

ResizeCommand::ResizeCommand(const EditorViewScene *scene, const Id &id
		, const QRectF &oldGeometry, const QRectF &newGeometry)
	: NodeElementCommand(scene, id)
{
	mOldGeometrySnapshot.insert(id, oldGeometry);
	mNewGeometrySnapshot.insert(id, newGeometry);
	// Marking that tracking already gone
	TrackingEntity::startTracking();
	TrackingEntity::stopTracking();
}

ResizeCommand *ResizeCommand::create(const NodeElement * const element
		, const QRectF &newContents, const QPointF &newPos
		, const QRectF &oldContents, const QPointF &oldPos)
{
	QRectF newContentsAndPos = newContents;
	newContentsAndPos.moveTo(newPos);
	QRectF oldContentsAndPos = oldContents;
	oldContentsAndPos.moveTo(oldPos);
	return new ResizeCommand(dynamic_cast<EditorViewScene *>(element->scene())
			, element->id(), oldContentsAndPos, newContentsAndPos);
}

bool ResizeCommand::execute()
{
	if (!mTrackStopped) {
		return true;
	}

	if (!NodeElementCommand::execute()) {
		return false;
	}

	resizeHierarchy(mNewGeometrySnapshot);
	return true;
}

bool ResizeCommand::restoreState()
{
	if (!mTrackStopped) {
		return true;
	}

	if (!NodeElementCommand::restoreState()) {
		return false;
	}

	resizeHierarchy(mOldGeometrySnapshot);
	return true;
}

void ResizeCommand::resizeHierarchy(QMap<Id, QRectF> const &snapshot)
{
	for (const Id &id : snapshot.keys()) {
		NodeElement *element = nodeById(id);
		if (!element->parentItem()) {
			resizeTree(snapshot, id);
		}
	}

	// Updating linker position
	if (mScene->selectedItems().size() == 1) {
		QGraphicsItem *selectedItem = mScene->selectedItems()[0];
		NodeElement *selectedNode = dynamic_cast<NodeElement *>(selectedItem);
		if (selectedNode) {
			selectedNode->setVisibleEmbeddedLinkers(true);
		}
	}
}

void ResizeCommand::resizeTree(QMap<Id, QRectF> const &snapshot, const Id &root)
{
	NodeElement *element = nodeById(root);
	for (NodeElement *child : element->childNodes()) {
		if (snapshot.contains(child->id())) {
			resizeTree(snapshot, child->id());
		}
	}

	resize(element, snapshot[root]);
}

void ResizeCommand::resize(NodeElement * const element, const QRectF &geometry)
{
	if (element && geometryOf(element) != geometry) {
		ResizeHandler handler(*element);
		handler.resize(geometry.translated(-geometry.topLeft()), geometry.topLeft(), false);
	}
}

void ResizeCommand::startTracking()
{
	NodeElementCommand::reinitElement();
	if (!mNode) {
		return;
	}

	TrackingEntity::startTracking();
	makeCommonSnapshot(mOldGeometrySnapshot);
	startEdgeTracking();
}

void ResizeCommand::stopTracking()
{
	TrackingEntity::stopTracking();
	makeCommonSnapshot(mNewGeometrySnapshot);
	stopEdgeTracking();
}

void ResizeCommand::rejectTracking()
{
	TrackingEntity::stopTracking();
}

void ResizeCommand::makeCommonSnapshot(QMap<Id, QRectF> &target)
{
	/// This must be invoked even if we start element dragging when it isn`t selected
	makeHierarchySnapshot(mNode, target);
	const QList<QGraphicsItem *> selectedItems = mNode->scene()->selectedItems();
	for (QGraphicsItem *const item : selectedItems) {
		NodeElement * const node = dynamic_cast<NodeElement *>(item);
		if (node && node != mNode) {
			makeHierarchySnapshot(node, target);
		} else {
			EdgeElement * const edge = dynamic_cast<EdgeElement *>(item);
			if (edge) {
				mEdges.insert(edge);
			}
		}
	}
}

void ResizeCommand::makeHierarchySnapshot(NodeElement *node, QMap<Id, QRectF> &target)
{
	// Here we remembering all binded items geometries.
	// Binded items are just element`s hierarchy:
	// all parents and children (siblings are not considered)
	makeChildrenSnapshot(node, target);
	for (NodeElement *parentElement = node; parentElement;
			parentElement = dynamic_cast<NodeElement *>(parentElement->parentItem()))
	{
		target.insert(parentElement->id(), geometryOf(parentElement));
		addEdges(parentElement);
	}
}

void ResizeCommand::makeChildrenSnapshot(const NodeElement *element, QMap<Id, QRectF> &target)
{
	target.insert(element->id(), geometryOf(element));
	addEdges(element);
	for (const QGraphicsItem * const childItem : element->childItems()) {
		const NodeElement * const child = dynamic_cast<const NodeElement * const>(childItem);
		if (child) {
			makeChildrenSnapshot(child, target);
		}
	}
}

void ResizeCommand::addEdges(const NodeElement *node)
{
	for (EdgeElement * const edge : node->getEdges()) {
		mEdges.insert(edge);
	}
}

void ResizeCommand::startEdgeTracking()
{
	for (EdgeElement * const edge : mEdges) {
		ReshapeEdgeCommand *reshapeCommand = new ReshapeEdgeCommand(edge);
		mEdgeCommands << reshapeCommand;
		reshapeCommand->startTracking();
		addPostAction(reshapeCommand);
	}
}

void ResizeCommand::stopEdgeTracking()
{
	for (ReshapeEdgeCommand * const command : mEdgeCommands) {
		command->stopTracking();
	}
}

QRectF ResizeCommand::geometryOf(const NodeElement *element) const
{
	const QRectF geom = element->contentsRect();
	return geom.translated(element->pos() - geom.topLeft());
}

QRectF ResizeCommand::geometryBeforeDrag() const
{
	return mOldGeometrySnapshot[mId];
}

bool ResizeCommand::modificationsHappened() const
{
	return mOldGeometrySnapshot != mNewGeometrySnapshot;
}
