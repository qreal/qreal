#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>

#include <qrgui/controller/commands/abstractCommand.h>
#include <qrgui/controller/commands/trackingEntity.h>

namespace qReal {
namespace shapeEdit {

class MoveItemCommand : public commands::AbstractCommand, public commands::TrackingEntity
{
public:
    MoveItemCommand(QGraphicsScene *scene, QList<QGraphicsItem *> items);

    void startTracking();
    void stopTracking();

    bool isMoved() const;

protected:
    bool execute();
    bool restoreState();

private:
    QGraphicsScene *mScene;
    QList<QGraphicsItem *> mItems;
    bool mIsExecuted;

    QList<QPointF> mInitPoss;
    QList<QPointF> mLastPoss;
};

}
}
