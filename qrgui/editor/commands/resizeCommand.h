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

#include <QtCore/QMap>

#include "editor/private/resizeHandler.h"
#include "editor/commands/reshapeEdgeCommand.h"
#include "editor/commands/nodeElementCommand.h"
#include "controller/commands/trackingEntity.h"

namespace qReal {
namespace gui {
namespace editor {
namespace commands {

class ResizeCommand : public NodeElementCommand, public qReal::commands::TrackingEntity
{
public:
	/// Creating new instance of command in tracking-based style
	ResizeCommand(const EditorViewScene *scene, const Id &id);

	/// Creating new instance of command in tracking-based style
	ResizeCommand(const EditorView *view, const Id &id);

	/// Creating new instance of command in usial style
	ResizeCommand(const EditorViewScene *scene, const Id &id
			, const QRectF &oldGeometry, const QRectF &newGeometry);
	virtual ~ResizeCommand() {}

	static ResizeCommand *create(const NodeElement * const element, const QRectF &newContents
			, const QPointF &newPos, const QRectF &oldContents, const QPointF &oldPos);

	virtual void startTracking();
	virtual void stopTracking();
	virtual void rejectTracking();

	QRectF geometryBeforeDrag() const;
	bool modificationsHappened() const;

protected:
	bool execute();
	bool restoreState();

private:
	void resize(NodeElement * const element, const QRectF &geometry);

	/// Performs geometries snapshot for all selected items hierarchies
	void makeCommonSnapshot(QMap<Id, QRectF> &target);

	/// Performs geometries snapshot for specified item`s hierarchy: its parent and child items
	void makeHierarchySnapshot(NodeElement *node, QMap<Id, QRectF> &target);

	/// Performs geometries snapshot for specified item`s children
	void makeChildrenSnapshot(const NodeElement *element, QMap<Id, QRectF> &target);

	void addEdges(const NodeElement *node);
	void startEdgeTracking();
	void stopEdgeTracking();

	void resizeHierarchy(QMap<Id, QRectF> const &snapshot);
	void resizeTree(QMap<Id, QRectF> const &snapshot, const Id &root);

	QRectF geometryOf(const NodeElement *element) const;

	QMap<Id, QRectF> mOldGeometrySnapshot;
	QMap<Id, QRectF> mNewGeometrySnapshot;

	QSet<EdgeElement *> mEdges;
	QSet<ReshapeEdgeCommand *> mEdgeCommands;
};

}
}
}
}
