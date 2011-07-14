#include "tableDisposer.h"

#include <QGraphicsProxyWidget>

TableDisposer::TableDisposer() {}

QMap<Element*, QGraphicsItem*> TableDisposer::switches = QMap<Element*, QGraphicsItem*>();
QMap<Element*, bool> TableDisposer::folded = QMap<Element*, bool>();

/* Public */

bool TableDisposer::isFolded(Element *element) const {
	initFoldedGlobalState(element);
	return folded.take(element);
}

void TableDisposer::switchFolding(const bool request, Element *element, QList<Item> &items, QRectF &boundingRectToCompute) const {
	Q_UNUSED(items);
	Q_UNUSED(boundingRectToCompute);
	initSwitchesGlobalState(element, items, boundingRectToCompute);

//	QGraphicsItem* switchItem = switches.take(element);
	if (!request) {
		folded.insert(element, false);
		dispose(element, items, boundingRectToCompute);
//		switchItem->setPos(switchItem->pos() + QPointF(-5,-5));
	} else {
		folded.insert(element, true);
		fold(element, items, boundingRectToCompute);
//		switchItem->setPos(switchItem->pos() + QPointF(+5,+5));
	}
}

/* Initialization */

void TableDisposer::initFoldedGlobalState(Element *element) {
	if (!folded.contains(element)) {
		folded.insert(element, true);
	}
}

void TableDisposer::initSwitchesGlobalState(Element *element, QList<Item> &items, QRectF &boundingRectToCompute) {
	Q_UNUSED(items);
	Q_UNUSED(boundingRectToCompute);
	if (!switches.contains(element)) {
//		switches.insert(element, new QGraphicsRectItem(-5,-5,+5,+5, element));
	}
}

/* Folding */

void TableDisposer::fold(Element *element, QList<Item> &items, QRectF &boundingRectToCompute) const {
	foreach(Item forldButton, items) {
		QGraphicsProxyWidget* widget = forldButton.second;
		widget->hide();
	}
}

/* Disposing */

void TableDisposer::dispose(Element *element, QList<Item> &items, QRectF &boundingRectToCompute) const {
	const qreal stretching = 2;

	const QRectF elementBounding = element->boundingRect();
	const qreal length  = elementBounding.right() - elementBounding.left();
	const qreal middleX = elementBounding.left()  + length/2;
	const qreal bottom  = elementBounding.bottom();

	qSort(items);

	const qreal avrLength = items.at(items.length()/2).boundingLength();
	const qreal maxLength = items.last().boundingLength();
	const qreal sumLength = avrLength + maxLength;
	const qreal treshold  = length*stretching;

	if (sumLength <= treshold && items.length() > 1) {
		boundingRectToCompute = disposeInColumns(items, 2, sumLength, middleX, bottom);
	} else {
		boundingRectToCompute = disposeInColumns(items, 1, maxLength, middleX, bottom);
	}
}

QRectF TableDisposer::disposeInColumns(QList<Item> &items, const int columns, const qreal length,
										const qreal middleX, const qreal top) const {
	const qreal left = middleX - length/2;
	const int number = items.length();
	const int rows  = number / columns;
	int remaind = number % columns;

	int i = 0;
	QRectF rect;

	qreal curY;
	qreal curX = left;
	for (int column = 0; column < columns; column++) {
		curY  = top;
		for (int row = 0; row < rows; row++) {
			disposeOne(items, rect, curX, curY, i);
		}
		if (remaind > 0) {
			disposeOne(items, rect, curX, curY, i);
		}
		curX += rect.width();
	}

	return QRectF(left, top, curX - left, curY - top);
}

void TableDisposer::disposeOne(QList<Item> &items, QRectF &rect, qreal &curX, qreal &curY, int &i) const {
	const Item item = items.at(i);
	QGraphicsProxyWidget* widget = item.second;
	rect = widget->boundingRect();
	widget->setPos(curX, curY);
	curY += rect.height();
	widget->show();
	i++;
}
