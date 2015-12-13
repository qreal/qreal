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

#include "reshapeEdgeCommand.h"

using namespace qReal::commands;
using namespace qReal::gui::editor::commands;

ReshapeEdgeCommand::ReshapeEdgeCommand(const EdgeElement *edge)
	: EdgeElementCommand(dynamic_cast<EditorViewScene *>(edge->scene()), edge->id())
{
}

ReshapeEdgeCommand::ReshapeEdgeCommand(const EditorViewScene *scene, const Id &id)
	: EdgeElementCommand(scene, id)
{
}

bool ReshapeEdgeCommand::execute()
{
	if (!mTrackStopped) {
		return true;
	}

	if (!EdgeElementCommand::execute()) {
		return false;
	}

	applyConfiguration(mNewConfiguration, mNewSrc, mNewDst, mNewPos, mNewFromPort, mNewToPort);
	return true;
}

bool ReshapeEdgeCommand::restoreState()
{
	if (!mTrackStopped) {
		return true;
	}

	if (!EdgeElementCommand::restoreState()) {
		return false;
	}

	applyConfiguration(mOldConfiguration, mOldSrc, mOldDst, mOldPos, mOldFromPort, mOldToPort);
	return true;
}

void ReshapeEdgeCommand::startTracking()
{
	EdgeElementCommand::reinitElement();
	TrackingEntity::startTracking();
	saveConfiguration(mOldConfiguration, mOldSrc, mOldDst, mOldPos, mOldFromPort, mOldToPort);
}

void ReshapeEdgeCommand::stopTracking()
{
	EdgeElementCommand::reinitElement();
	TrackingEntity::stopTracking();
	saveConfiguration(mNewConfiguration, mNewSrc, mNewDst, mNewPos, mNewFromPort, mNewToPort);
}

bool ReshapeEdgeCommand::somethingChanged() const
{
	return mOldConfiguration != mNewConfiguration
			|| mOldPos != mNewPos
			|| mOldSrc != mNewSrc
			|| mOldDst != mNewDst
			|| mOldFromPort != mNewFromPort
			|| mOldToPort != mNewToPort;
}

void ReshapeEdgeCommand::saveConfiguration(QPolygonF &target, Id &src, Id &dst
		, QPointF &pos, qreal &fromPort, qreal &toPort)
{
	if (mEdge) {
		target = mEdge->line();
		src = mEdge->src() ? mEdge->src()->id() : Id();
		dst = mEdge->dst() ? mEdge->dst()->id() : Id();
		pos = mEdge->pos();
		fromPort = mEdge->fromPort();
		toPort = mEdge->toPort();
	}
}

void ReshapeEdgeCommand::applyConfiguration(const QPolygonF &configuration
		, const Id &src, const Id &dst, const QPointF &pos, const qreal &fromPort, const qreal &toPort)
{
	if (!mEdge) {
		return;
	}

	NodeElement *srcElem = dynamic_cast<NodeElement *>(elementById(src));
	NodeElement *dstElem = dynamic_cast<NodeElement *>(elementById(dst));
	mEdge->setSrc(srcElem);
	mEdge->setDst(dstElem);
	mEdge->setLine(configuration);
	mEdge->setFromPort(fromPort);
	mEdge->setToPort(toPort);
	mEdge->arrangeLinearPorts();
	mEdge->setPos(pos);
	mEdge->scene()->update();
}
