#pragma once

#include <QGraphicsScene>
#include <QGraphicsItem>

#include <qrgui/controller/commands/abstractCommand.h>

namespace qReal {
namespace shapeEdit {

class RemoveItemCommand : public commands::AbstractCommand
{

public:
    RemoveItemCommand(QGraphicsScene *scene, QGraphicsItem *item);
    ~RemoveItemCommand();

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
