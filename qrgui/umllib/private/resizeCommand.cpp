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
}

void ResizeCommand::resize(NodeElement * const element, QRectF const &geometry)
{
	if (element && geometryOf(element) != geometry) {
		ResizeHandler handler(element);
		handler.resize(geometry.translated(-geometry.topLeft()), geometry.topLeft());
	}
}

void ResizeCommand::startTracking()
{
	NodeElementCommand::reinitElement();
	if (!mNode) {
		return;
	}
	TrackingEntity::startTracking();
	makeHierarchySnapshot(mOldGeometrySnapshot);
}

void ResizeCommand::stopTracking()
{
	TrackingEntity::stopTracking();
	makeHierarchySnapshot(mNewGeometrySnapshot);
}

void ResizeCommand::rejectTracking()
{
	TrackingEntity::stopTracking();
}

void ResizeCommand::makeHierarchySnapshot(QMap<Id, QRectF> &target)
{
	// Here we remembering all binded items geometries.
	// Binded items are just element`s hierarchy:
	// all parents and children (siblings are not considered)
	makeChildrenSnapshot(mNode, target);
	for (NodeElement *parentElement = mNode; parentElement;
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
//	return element->contentsRect().translated(element->pos());
}

QRectF ResizeCommand::geometryBeforeDrag() const
{
	return mOldGeometrySnapshot[mId];
}
