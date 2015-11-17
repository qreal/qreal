#include "addItemCommand.h"

using namespace qReal::commands;
using namespace qReal::shapeEdit;

AddItemCommand::AddItemCommand(Scene *scene, Item *item)
    : mScene(scene)
    , mItem(item)
{}

AddItemCommand::~AddItemCommand()
{
    delete mItem;
}

bool AddItemCommand::execute()
{
    mScene->addItem(mItem);
    return true;
}

bool AddItemCommand::restoreState()
{
    mScene->removeItem(mItem);
    return true;
}
