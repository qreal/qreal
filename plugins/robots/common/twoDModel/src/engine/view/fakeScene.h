#pragma once

#include <QtCore/QMap>
#include <QtWidgets/QGraphicsScene>

namespace twoDModel {

namespace model {
class WorldModel;
}

namespace view {

/// A scene that maintains a copy of the visible world for rendering some pieces of that (for sensors, for example).
class FakeScene : public QGraphicsScene
{
public:
	explicit FakeScene(const model::WorldModel &world);

	/// Renders a given piece of the scene and returns resulting image.
	QImage render(const QRectF &piece);

private:
	void addClone(QGraphicsItem * const original, QGraphicsItem * const cloned);
	void deleteItem(QGraphicsItem * const original);

	QMap<QGraphicsItem *, QGraphicsItem *> mClonedItems;
};

}
}
