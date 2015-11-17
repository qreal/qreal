#pragma once

#include "qrgui/controller/commands/abstractCommand.h"

#include "qrgui/mainWindow/shapeEdit/scene.h"
#include "qrgui/mainWindow/shapeEdit/item/item.h"

namespace qReal {
namespace shapeEdit {

/// The only purpose and ability:
/// adding (removing) @param item to (from) the @param scene.
class AddItemCommand : public commands::AbstractCommand
{

public:
    AddItemCommand(Scene *scene, Item *item);
    virtual ~AddItemCommand();

protected:
    bool execute();
    bool restoreState();

private:
    Scene *mScene;
    Item *mItem;
};

}
}
