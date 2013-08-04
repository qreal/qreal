#include "reshapeEdgeCommand.h"

using namespace qReal::commands;

ReshapeEdgeCommand::ReshapeEdgeCommand(EdgeElement const *edge)
	: EdgeElementCommand(dynamic_cast<EditorViewScene *>(edge->scene()), edge->id())
{
}

ReshapeEdgeCommand::ReshapeEdgeCommand(EditorViewScene const *scene, Id const &id)
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
	applyConfiguration(mNewConfiguration, mNewSrc, mNewDst, mNewPos);
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
	applyConfiguration(mOldConfiguration, mOldSrc, mOldDst, mOldPos);
	return true;
}

void ReshapeEdgeCommand::startTracking()
{
	EdgeElementCommand::reinitElement();
	TrackingEntity::startTracking();
	saveConfiguration(mOldConfiguration, mOldSrc, mOldDst, mOldPos);
}

void ReshapeEdgeCommand::stopTracking()
{
	EdgeElementCommand::reinitElement();
	TrackingEntity::stopTracking();
	saveConfiguration(mNewConfiguration, mNewSrc, mNewDst, mNewPos);
}

void ReshapeEdgeCommand::saveConfiguration(QPolygonF &target, Id &src, Id &dst
		, QPointF &pos)
{
	if (mEdge) {
		target = mEdge->line();
		src = mEdge->src() ? mEdge->src()->id() : Id();
		dst = mEdge->dst() ? mEdge->dst()->id() : Id();
		pos = mEdge->pos();
	}
}

void ReshapeEdgeCommand::applyConfiguration(QPolygonF const &configuration
		, Id const &src, Id const &dst, QPointF const &pos)
{
	if (!mEdge) {
		return;
	}
	NodeElement *srcElem = dynamic_cast<NodeElement *>(elementById(src));
	NodeElement *dstElem = dynamic_cast<NodeElement *>(elementById(dst));
	mEdge->setLine(configuration);
	mEdge->setSrc(srcElem);
	mEdge->setDst(dstElem);
	mEdge->setPos(pos);
	mEdge->connectToPort();
	if (srcElem) {
		srcElem->arrangeLinks();
		srcElem->adjustLinks();
	}
	if (dstElem) {
		dstElem->arrangeLinks();
		dstElem->adjustLinks();
	}
	mEdge->scene()->update();
}
