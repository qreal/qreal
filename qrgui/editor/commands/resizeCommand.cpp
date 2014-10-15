#include "editor/commands/resizeCommand.h"

using namespace qReal::commands;

ResizeCommand::ResizeCommand(EditorViewScene const *scene, Id const &id)
	: NodeElementCommand(scene, id)
{
}

ResizeCommand::ResizeCommand(EditorView const *view, Id const &id)
	: NodeElementCommand(&view->editorViewScene(), id)
{
}

ResizeCommand::ResizeCommand(EditorViewScene const *scene, Id const &id
		, QRectF const &oldGeometry, QRectF const &newGeometry)
	: NodeElementCommand(scene, id)
{
	mOldGeometrySnapshot.insert(id, oldGeometry);
	mNewGeometrySnapshot.insert(id, newGeometry);
	// Marking that tracking already gone
	TrackingEntity::startTracking();
	TrackingEntity::stopTracking();
}

ResizeCommand *ResizeCommand::create(NodeElement const * const element
		, QRectF const &newContents, QPointF const &newPos
		, QRectF const &oldContents, QPointF const &oldPos)
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
	foreach (Id const &id, snapshot.keys()) {
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

void ResizeCommand::resizeTree(QMap<Id, QRectF> const &snapshot, Id const &root)
{
	NodeElement *element = nodeById(root);
	foreach (NodeElement *child, element->childNodes()) {
		if (snapshot.contains(child->id())) {
			resizeTree(snapshot, child->id());
		}
	}
	resize(element, snapshot[root]);
}

void ResizeCommand::resize(NodeElement * const element, QRectF const &geometry)
{
	if (element && geometryOf(element) != geometry) {
		ResizeHandler handler(element);
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
	QList<QGraphicsItem *> const selectedItems = mNode->scene()->selectedItems();
	foreach (QGraphicsItem *const item, selectedItems) {
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

void ResizeCommand::makeChildrenSnapshot(NodeElement const *element, QMap<Id, QRectF> &target)
{
	target.insert(element->id(), geometryOf(element));
	addEdges(element);
	foreach (QGraphicsItem const * const childItem, element->childItems()) {
		NodeElement const * const child = dynamic_cast<NodeElement const * const>(childItem);
		if (child) {
			makeChildrenSnapshot(child, target);
		}
	}
}

void ResizeCommand::addEdges(NodeElement const *node)
{
	foreach (EdgeElement * const edge, node->getEdges()) {
		mEdges.insert(edge);
	}
}

void ResizeCommand::startEdgeTracking()
{
	foreach (EdgeElement * const edge, mEdges) {
		ReshapeEdgeCommand *reshapeCommand = new ReshapeEdgeCommand(edge);
		mEdgeCommands << reshapeCommand;
		reshapeCommand->startTracking();
		addPostAction(reshapeCommand);
	}
}

void ResizeCommand::stopEdgeTracking()
{
	foreach (ReshapeEdgeCommand * const command, mEdgeCommands) {
		command->stopTracking();
	}
}

QRectF ResizeCommand::geometryOf(NodeElement const *element) const
{
	QRectF const geom = element->contentsRect();
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
