#pragma once

#include <QGraphicsSceneMouseEvent>

#include <qrgui/controller/commands/abstractCommand.h>
#include <qrgui/controller/commands/trackingEntity.h>

#include "qrgui/mainWindow/shapeEdit/scene.h"
#include "qrgui/mainWindow/shapeEdit/item/item.h"

namespace qReal {
namespace shapeEdit {

class ResizeItemCommand : public commands::AbstractCommand
{
public:
    ResizeItemCommand(Scene *scene, QGraphicsSceneMouseEvent *pressEvent, QGraphicsSceneMouseEvent *releaseEvent
                      , bool isExecuted = true);

    ~ResizeItemCommand();

protected:
    bool execute();
    bool restoreState();

private:
    Scene *mScene;
    QGraphicsSceneMouseEvent *mPressEvent;
    QGraphicsSceneMouseEvent *mReleaseEvent;
    bool mIsExecuted;

    void executeEvents(QGraphicsSceneMouseEvent *firstEvent, QGraphicsSceneMouseEvent *secondEvent);
};

}
}
