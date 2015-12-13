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

#include "editor/private/lineHandler.h"

namespace qReal {
namespace gui {
namespace editor {

/// @brief A strategy class for handling custom broken link
/// User may move points of such link, create new points, delete points and segments
class BrokenLine : public LineHandler
{
	Q_OBJECT
public:
	BrokenLine(EdgeElement *edge);

	/// Align every non-end point of the link to grid
	virtual void alignToGrid();

	/// @return List of context menu actions that may be done on a link at the point pos
	virtual QList<ContextMenuAction *> extraActions(const QPointF &pos);

protected slots:
	/// Delete non-end point at position pos (given in link's coordinates), if any
	void deletePoint(const QPointF &pos);

	/// Delete non-end segment at position pos (given in link's coordinates), if any
	void deleteSegment(const QPointF &pos);

protected:
	/// Move point to position pos (given in link's coordinates)
	/// Point is determined earlier (when mouse button is pressed)
	/// If there is no point make the one then move it
	virtual void handleEdgeMove(const QPointF &pos);

	/// Add a new point at position pos, if there is a link's segment containing pos
	int addPoint(const QPointF &pos);

	/// Align point to the nearest grid corner
	QPointF alignedPoint(const QPointF &point) const;

	/// Remove unneeded points, lines and loops (if the link intersects itself)
	virtual void improveAppearance();

	/// Remove points and lines that are so close to each other that look like single point or line
	void deleteUnneededPoints();

	/// If two or more points are too close to each other replace them with just one point
	void deleteClosePoints(QPolygonF &line) const;

	/// Remove one of two lines if they are very close to each other
	void removeSmallTriangles(QPolygonF &line) const;

	/// Determine whether points of line at positions i - i+2 look like one line rather than two
	bool tooSmallTriangle(const QPolygonF &line, int i) const;

	/// @return true if pos is close enough to non-end point
	bool delPointActionIsPossible(const QPointF &pos) const;

	/// @return true if pos is close enough to non-end segment
	bool delSegmentActionIsPossible(const QPointF &pos);

	/// @return true if link consists of more than 2 points
	bool minimizeActionIsPossible() const;

	ContextMenuAction mDeletePointAction;
	ContextMenuAction mDeleteSegmentAction;
	ContextMenuAction mMinimizeAction;
};

}
}
}
