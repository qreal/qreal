#include "embeddedControls.h"

#include "enumWidget.h"
#include "booleanWidget.h"
#include "numericWidget.h"
#include "layoutedControl.h"

#include "tableDisposer.h"

#include "../../edgeElement.h"

#include <QtAlgorithms>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

QMap<Element*, EmbeddedControls*> EmbeddedControls::elementsToControls;

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
	return computedBoundingRect;
}

void EmbeddedControls::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	painter->save();
//	painter->drawRect(boundingRect().left()-5,boundingRect().top()-5,boundingRect().width()+10,boundingRect().height()+10);
	painter->restore();
}

/* Private */

EmbeddedControls::EmbeddedControls(Element *element, const qReal::EditorManager &editorManager) :
	QGraphicsItem(element),
	element(element)
{
	disposer = NULL;	//todo: надо загружать из настроек
	registerControls(element);
	initializeItems(element, editorManager);
	switchFolding(true);
}

void EmbeddedControls::registerControls(Element *element) {
	if (elementsToControls.contains(element)) {
		const EmbeddedControls* prev = elementsToControls.value(element);
		elementsToControls.remove(element);
		delete prev;
	}
	elementsToControls.insert(element, this);
	connect(element, SIGNAL(switchFolding(bool)), this, SLOT(switchFolding(bool)));
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
		proxy->hide();

		items.append(Item(wrapper->getControl(), proxy));
	}
}

void EmbeddedControls::switchFolding(const bool request) {
	if (items.isEmpty()) {
		return;
	}

	if (disposer == NULL) {
		disposer = new TableDisposer();//todo: переделать на настройки
	}

	const bool current = disposer->isFolded(element);
	if (current == request) {
		return;
	}

	disposer->switchFolding(request, element, items, computedBoundingRect);
	emit foldingSwitched(request);
}

/* Static */

void EmbeddedControls::loadSettings() {
	//todo
}

bool EmbeddedControls::checkPermission(Element *element) {
	//todo
	return true;
}

/* Assist */

qReal::Id EmbeddedControls::extractElementId(const qReal::Id &id) {
	const QString editor(id.editor());
	const QString diagram(id.diagram());
	const QString element(id.element());
	return qReal::Id(editor, diagram, element);
}
