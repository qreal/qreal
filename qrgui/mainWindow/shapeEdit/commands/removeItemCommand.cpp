#include "removeItemCommand.h"

using namespace qReal::commands;
using namespace qReal::shapeEdit;

RemoveItemCommand::RemoveItemCommand(QGraphicsScene *scene, QGraphicsItem *item, bool isExecuted)
    : mScene(scene)
    , mItem(item)
    , mIsExecuted(isExecuted)
{}

RemoveItemCommand::~RemoveItemCommand()
{
    if (mIsExecuted && !mItem->parentObject()) {
        // Item must be deleted because
        // in that case no owner of that item exist.
        delete mItem;
    }
}

bool RemoveItemCommand::execute()
{
    if (mItem && !mIsExecuted) {
        mScene->removeItem(mItem);
        mIsExecuted = true;
        return true;
    }
    return false;
}

bool RemoveItemCommand::restoreState()
{
    if (mItem && mIsExecuted) {
        mScene->addItem(mItem);
        mIsExecuted = false;
        return true;
    }
    return false;
}
