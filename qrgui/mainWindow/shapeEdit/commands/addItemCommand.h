#pragma once

#include <QGraphicsScene>
#include <QGraphicsItem>

#include <qrgui/controller/commands/abstractCommand.h>

namespace qReal {
namespace shapeEdit {

/// The only purpose and ability:
/// adding (removing) @param item to (from) the @param scene.
class AddItemCommand : public commands::AbstractCommand
{

public:
    AddItemCommand(QGraphicsScene *scene, QGraphicsItem *item, bool isExecuted = false);
    ~AddItemCommand();

protected:
    bool execute();
    bool restoreState();

private:
    QGraphicsScene *mScene;
    QGraphicsItem *mItem;
    bool mIsExecuted;
};

}
}
