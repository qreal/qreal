#include "resizeItemCommand.h"

using namespace qReal::commands;
using namespace qReal::shapeEdit;

ResizeItemCommand::ResizeItemCommand(Scene *scene, QGraphicsSceneMouseEvent *pressEvent, QGraphicsSceneMouseEvent *releaseEvent, bool isExecuted)
    : mScene(scene)
    , mIsExecuted(isExecuted)
    , mPressEvent(pressEvent)
    , mReleaseEvent(releaseEvent)
{}

ResizeItemCommand::~ResizeItemCommand()
{
    delete mPressEvent;
    delete mReleaseEvent;
}

bool ResizeItemCommand::execute()
{
    if (!mIsExecuted) {
        executeEvents(mPressEvent, mReleaseEvent);
        mIsExecuted = true;
    }
    return true;
}

bool ResizeItemCommand::restoreState()
{
    executeEvents(mReleaseEvent, mPressEvent);
    mIsExecuted = false;
    return true;
}

void ResizeItemCommand::executeEvents(QGraphicsSceneMouseEvent *firstEvent, QGraphicsSceneMouseEvent *secondEvent)
{
    mScene->forPressResize(firstEvent);
    Item *graphicsItem = dynamic_cast<Item *>(mScene->mGraphicsItem);
    if (graphicsItem) {
        graphicsItem->changeScalingPointState(mScene->mX1, mScene->mY1);
        if (graphicsItem->getScalingPointState() != Item::noneScale) {
            graphicsItem->setScalingPointColor();
        }
    }
    mScene->forReleaseResize(secondEvent);
}
