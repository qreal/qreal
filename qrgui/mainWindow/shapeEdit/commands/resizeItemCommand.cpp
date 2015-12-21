#include "resizeItemCommand.h"

using namespace qReal::shapeEdit;
using namespace qReal::commands;
using namespace graphicsUtils;

ResizeItemCommand::ResizeItemCommand(QGraphicsScene *scene, AbstractItem *item, bool isExecuted)
    : mScene(scene)
    , mItem(item)
    , mIsExecuted(isExecuted)
{}

void ResizeItemCommand::startTracking()
{
    TrackingEntity::startTracking();

    mInitGeometryP1 = QPointF(mItem->x1(), mItem->y1());
    mInitGeometryP2 = QPointF(mItem->x2(), mItem->y2());
}

void ResizeItemCommand::stopTracking()
{
    mNewGeometryP1 = QPointF(mItem->x1(), mItem->y1());
    mNewGeometryP2 = QPointF(mItem->x2(), mItem->y2());
    mDragState = mItem->dragState();

    mIsExecuted = true;
    TrackingEntity::stopTracking();
}

bool ResizeItemCommand::execute()
{
    if (!mTrackStopped) {
        return false;
    }

    if (!mTrackStarted && !mIsExecuted) {
        mItem->setDragState(mDragState);
        mItem->reverseOldResizingItem(mNewGeometryP1, mNewGeometryP2);
        mIsExecuted = true;
        mScene->update();
    }
    return mIsExecuted;
}

bool ResizeItemCommand::restoreState()
{
    if (!mTrackStopped) {
        return false;
    }

    if (!mTrackStarted && mIsExecuted) {
        mItem->setDragState(mDragState);
        mItem->reverseOldResizingItem(mInitGeometryP1, mInitGeometryP2);
        mIsExecuted = false;
        mScene->update();
    }
    return !mIsExecuted;
}
