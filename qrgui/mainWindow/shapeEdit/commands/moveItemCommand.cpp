#include "moveItemCommand.h"

using namespace qReal::commands;
using namespace qReal::shapeEdit;

MoveItemCommand::MoveItemCommand(QGraphicsScene *scene, QList<QGraphicsItem *> items)
    : mScene(scene)
    , mItems(items)
    , mIsExecuted(false)
{}

void MoveItemCommand::startTracking()
{
    TrackingEntity::startTracking();

    for (auto item : mItems) {
        mInitPoss.push_back(item->scenePos());
    }
}

void MoveItemCommand::stopTracking()
{
    for (auto item : mItems) {
        mLastPoss.push_back(item->scenePos());
    }

    mIsExecuted = true;
    TrackingEntity::stopTracking();
}

bool MoveItemCommand::isMoved() const
{
    for (int i = 0; i < mItems.length(); ++i) {
        if (mInitPoss[i] != mLastPoss[i]) {
            return true;
        }
    }
    return false;
}

bool MoveItemCommand::execute()
{
    if (!mTrackStopped) {
        return false;
    }

    if (!mTrackStarted && !mIsExecuted) {
        for (int i = 0; i < mItems.length(); ++i) {
            mItems[i]->setPos(mLastPoss.at(i));
        }
        mScene->update();
        mIsExecuted = true;
    }
    return mIsExecuted;
}

bool MoveItemCommand::restoreState()
{
    if (!mTrackStopped) {
        return false;
    }

    if (!mTrackStarted && mIsExecuted) {
        for (int i = 0; i < mItems.length(); ++i) {
            mItems[i]->setPos(mInitPoss.at(i));
        }
        mScene->update();
        mIsExecuted = false;
    }
    return !mIsExecuted;
}
