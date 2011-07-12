#include "embeddedControls.h"

#include "enumWidget.h"
#include "booleanWidget.h"
#include "numericWidget.h"
#include "layoutedControl.h"

#include "../edgeElement.h"

#include <QtAlgorithms>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

ElementsToControls EmbeddedControls::elementsToControls;

/* Public */

bool EmbeddedControls::existsInstance(Element *element) {
	return elementsToControls.contains(element);
}

void EmbeddedControls::deleteInstance(Element *element) {
	if (elementsToControls.contains(element)) {
		const EmbeddedControls* prev = elementsToControls.value(element);
		delete prev;
		elementsToControls.remove(element);
	}
}

void EmbeddedControls::createInstance(Element *element, const qReal::EditorManager &editorManager) {
	new EmbeddedControls(element, editorManager);
}

QRectF EmbeddedControls::boundingRect() const {
	return computatedBoundingRect;
}

void EmbeddedControls::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	painter->save();

	if (!isEdge) {
		painter->drawRect(boundingRect().left()-5,boundingRect().top()-5,boundingRect().width()+10,boundingRect().height()+10);
	}

	painter->restore();
}

/* Private */

EmbeddedControls::EmbeddedControls(Element *element, const qReal::EditorManager &editorManager) :
	QGraphicsItem(element)
{
	registerControls(element);
	initializeElement(element);
	initializeItems(element, editorManager);
	disposeWidgets();
}

void EmbeddedControls::initializeElement(Element *element) {
	this->element = element;
	if (dynamic_cast<EdgeElement*>(element) == NULL) {
		isEdge = false;
	} else {
		isEdge = true;
	}
}

void EmbeddedControls::initializeItems(Element *element, const qReal::EditorManager &editorManager) {
	const qReal::Id &id = extractElementId(element->id());
	QGraphicsScene *scene = this->scene();

	foreach(const Item item, items) {
		scene->removeItem(item.second);
		delete item.second;
		delete item.first;
	}
	items.clear();

	foreach(const QString property, editorManager.getPropertyNames(id)) {
		const QStringList values = editorManager.getEnumValues(id, property);
		const QString type = editorManager.getTypeName(id, property);

		//todo: можно улучшить. вынеся класс Item в Control
		//todo: и перенастроив политику выдачи length

		LayoutedControl *wrapper;
		QGraphicsProxyWidget *proxy;
		if ("Bool" == type) {
			wrapper = new LayoutedControl(new BooleanWidget(element, property));
		} else if ("Int" == type) {
			wrapper = new LayoutedControl(new NumericWidget(element, property));
		} else if (!values.isEmpty()) {
			wrapper = new LayoutedControl(new EnumWidget(element, property, values));
		} else {
			return;
		}

		proxy = scene->addWidget(wrapper);
		proxy->setParentItem(this);

		items.append(Item(wrapper->getControl(), proxy));
	}
}

void EmbeddedControls::disposeWidgets() {
	if (items.isEmpty()) {
		return;
	}

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
		computatedBoundingRect = disposeWidgets(2, sumLength, middleX, bottom);
	} else {
		computatedBoundingRect = disposeWidgets(1, maxLength, middleX, bottom);
	}
}

QRectF EmbeddedControls::disposeWidgets(const int columns, const qreal length, const qreal middleX, const qreal top) {
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
			disposeWidget(rect, curX, curY, i);
		}
		if (remaind > 0) {
			disposeWidget(rect, curX, curY, i);
		}
		curX += rect.width();
	}

	return QRectF(left, top, curX - left, curY - top);
}

void EmbeddedControls::disposeWidget(QRectF &rect, qreal &curX, qreal &curY, int &i) {
	const Item item = items.at(i);
	QGraphicsProxyWidget* widget = item.second;
	rect = widget->boundingRect();
	widget->setPos(curX, curY);
	curY += rect.height();
	i++;
}

void EmbeddedControls::registerControls(Element *element) {
	if (elementsToControls.contains(element)) {
		const EmbeddedControls* prev = elementsToControls.value(element);
		if (prev != this) {
			delete prev;
			elementsToControls.remove(element);
			elementsToControls.insert(element, this);
		}
	} else {
		elementsToControls.insert(element, this);
	}
}

/* Assist */

qReal::Id EmbeddedControls::extractElementId(const qReal::Id &id) {
	const QString editor(id.editor());
	const QString diagram(id.diagram());
	const QString element(id.element());
	return qReal::Id(editor, diagram, element);
}
