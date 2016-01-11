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

#include "editor/commands/edgeElementCommand.h"
#include "controller/commands/trackingEntity.h"

namespace qReal
{
namespace gui {
namespace editor {
namespace commands
{

class ReshapeEdgeCommand : public EdgeElementCommand, public qReal::commands::TrackingEntity
{
public:
	ReshapeEdgeCommand(const EdgeElement *edge);
	ReshapeEdgeCommand(const EditorViewScene *scene, const Id &id);
	virtual ~ReshapeEdgeCommand() {}

	virtual void startTracking();
	virtual void stopTracking();

	bool somethingChanged() const;

protected:
	bool execute();
	bool restoreState();

private:
	void saveConfiguration(QPolygonF &target, Id &src, Id &dst, QPointF &pos, qreal &fromPort, qreal &toPort);
	void applyConfiguration(const QPolygonF &configuration, const Id &src
			, const Id &dst, const QPointF &pos, const qreal &fromPort, const qreal &toPort);

	QPolygonF mOldConfiguration;
	QPolygonF mNewConfiguration;
	QPointF mOldPos;
	QPointF mNewPos;
	Id mOldSrc;
	Id mOldDst;
	Id mNewSrc;
	Id mNewDst;
	qreal mOldFromPort;
	qreal mNewFromPort;
	qreal mOldToPort;
	qreal mNewToPort;
};

}
}
}
}
