#pragma once

#include <QtCore/QMap>

#include "editor/private/resizeHandler.h"
#include "editor/private/reshapeEdgeCommand.h"
#include "controller/commands/nodeElementCommand.h"
#include "controller/commands/trackingEntity.h"

namespace qReal
{
namespace commands
{

class ResizeCommand : public NodeElementCommand, public TrackingEntity
{
public:
	/// Creating new instance of command in tracking-based style
	ResizeCommand(EditorViewScene const *scene, Id const &id);

	/// Creating new instance of command in tracking-based style
	ResizeCommand(EditorView const *view, Id const &id);

	/// Creating new instance of command in usial style
	ResizeCommand(EditorViewScene const *scene, Id const &id
			, QRectF const &oldGeometry, QRectF const &newGeometry);
	virtual ~ResizeCommand() {}

	static ResizeCommand *create(NodeElement const * const element, QRectF const &newContents
			, QPointF const &newPos, QRectF const &oldContents, QPointF const &oldPos);

	virtual void startTracking();
	virtual void stopTracking();
	virtual void rejectTracking();

	QRectF geometryBeforeDrag() const;
	bool modificationsHappened() const;

protected:
	bool execute();
	bool restoreState();

private:
	void resize(NodeElement * const element, QRectF const &geometry);

	/// Performs geometries snapshot for all selected items hierarchies
	void makeCommonSnapshot(QMap<Id, QRectF> &target);

	/// Performs geometries snapshot for specified item`s hierarchy: its parent and child items
	void makeHierarchySnapshot(NodeElement *node, QMap<Id, QRectF> &target);

	/// Performs geometries snapshot for specified item`s children
	void makeChildrenSnapshot(NodeElement const *element, QMap<Id, QRectF> &target);

	void addEdges(NodeElement const *node);
	void startEdgeTracking();
	void stopEdgeTracking();

	void resizeHierarchy(QMap<Id, QRectF> const &snapshot);
	void resizeTree(QMap<Id, QRectF> const &snapshot, Id const &root);

	QRectF geometryOf(NodeElement const *element) const;

	QMap<Id, QRectF> mOldGeometrySnapshot;
	QMap<Id, QRectF> mNewGeometrySnapshot;

	QSet<EdgeElement *> mEdges;
	QSet<ReshapeEdgeCommand *> mEdgeCommands;
};

}
}
