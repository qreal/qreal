#pragma once

#include <QtCore/QMap>

#include "resizeHandler.h"
#include "../../controller/commands/nodeElementCommand.h"
#include "../../controller/commands/trackingEntity.h"

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

protected:
	bool execute();
	bool restoreState();

private:
	void resize(NodeElement * const element, QRectF const &geometry);
	void makeHierarchySnapshot(QMap<Id, QRectF> &target);
	void makeChildrenSnapshot(NodeElement const *element, QMap<Id, QRectF> &target);

	void resizeHierarchy(QMap<Id, QRectF> const &snapshot);

	QRectF geometryOf(NodeElement const *element) const;

	QMap<Id, QRectF> mOldGeometrySnapshot;
	QMap<Id, QRectF> mNewGeometrySnapshot;
};

}
}
