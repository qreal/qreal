#include "resizeCommand.h"

using namespace qReal::commands;

ResizeCommand::ResizeCommand(EditorViewScene const *scene, Id const &id)
	: NodeElementCommand(scene, id)
{
}

ResizeCommand::ResizeCommand(EditorView const *view, Id const &id)
	: NodeElementCommand(view->editorViewScene(), id)
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
	NodeElementCommand::execute();
	resizeHierarchy(mNewGeometrySnapshot);
	return true;
}

bool ResizeCommand::restoreState()
{
	if (!mTrackStopped) {
		return true;
	}
	NodeElementCommand::restoreState();
	resizeHierarchy(mOldGeometrySnapshot);
	return true;
}

void ResizeCommand::resizeHierarchy(QMap<Id, QRectF> const &snapshot)
{
	foreach (Id const &id, snapshot.keys()) {
		NodeElement *element = nodeById(id);
		resize(element, snapshot[id]);
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

void ResizeCommand::resize(NodeElement * const element, QRectF const &geometry)
{
	if (element && geometryOf(element) != geometry) {
		ResizeHandler handler(element);
		handler.resize(geometry.translated(-geometry.topLeft()), geometry.topLeft(), geometry.topLeft());
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
}

void ResizeCommand::stopTracking()
{
	TrackingEntity::stopTracking();
	makeCommonSnapshot(mNewGeometrySnapshot);
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
	foreach (QGraphicsItem *item, selectedItems) {
		NodeElement *node = dynamic_cast<NodeElement *>(item);
		if (node && node != mNode) {
			makeHierarchySnapshot(node, target);
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
	}
}

void ResizeCommand::makeChildrenSnapshot(NodeElement const *element, QMap<Id, QRectF> &target)
{
	target.insert(element->id(), geometryOf(element));
	foreach (QGraphicsItem const *childItem, element->childItems()) {
		NodeElement const *child = dynamic_cast<NodeElement const *>(childItem);
		if (child) {
			makeChildrenSnapshot(child, target);
		}
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
