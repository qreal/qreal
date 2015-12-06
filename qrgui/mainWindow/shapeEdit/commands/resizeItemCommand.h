#pragma once

#include <QGraphicsScene>

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrgui/controller/commands/abstractCommand.h>
#include <qrgui/controller/commands/trackingEntity.h>

namespace qReal {
namespace shapeEdit {

class ResizeItemCommand : public commands::AbstractCommand, public commands::TrackingEntity
{
public:
    ResizeItemCommand(QGraphicsScene *scene, graphicsUtils::AbstractItem *item, bool isExecuted = true);

    void startTracking();
    void stopTracking();

protected:
    bool execute();
    bool restoreState();

private:
    QGraphicsScene *mScene;
    graphicsUtils::AbstractItem *mItem;
    bool mIsExecuted;
    graphicsUtils::AbstractItem::DragState mDragState;
    QPointF mInitGeometryP1;
    QPointF mInitGeometryP2;
    QPointF mNewGeometryP1;
    QPointF mNewGeometryP2;
};

}
}
