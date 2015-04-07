#include "fakeScene.h"

#include "src/engine/model/worldModel.h"
#include "src/engine/items/wallItem.h"
#include "src/engine/items/colorFieldItem.h"

using namespace twoDModel;
using namespace view;
using namespace model;

FakeScene::FakeScene(const WorldModel &world)
{
	connect(&world, &WorldModel::wallAdded, [=](items::WallItem *wall) { addClone(wall, wall->clone()); });
	connect(&world, &WorldModel::colorItemAdded, [=](items::ColorFieldItem *item) { addClone(item, item->clone()); });
	connect(&world, &WorldModel::otherItemAdded, [=](QGraphicsItem *item) {
		if (QGraphicsLineItem * const trace = dynamic_cast<QGraphicsLineItem *>(item)) {
			addClone(item, new QGraphicsLineItem(trace->line()));
		}
	});
	connect(&world, &WorldModel::itemRemoved, this, &FakeScene::deleteItem);
}

void FakeScene::addClone(QGraphicsItem * const original, QGraphicsItem * const cloned)
{
	mClonedItems[original] = cloned;
	addItem(cloned);
}

void FakeScene::deleteItem(QGraphicsItem * const original)
{
	if (mClonedItems.contains(original)) {
		delete mClonedItems[original];
		mClonedItems.remove(original);
	}
}

QImage view::FakeScene::render(const QRectF &piece)
{
	QImage result(piece.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&result);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::white));
	painter.drawRect(piece.translated(-piece.topLeft()));

	QGraphicsScene::render(&painter, QRectF(), piece);

	return result;
}
