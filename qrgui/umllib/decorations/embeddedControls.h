#pragma once

#include <QSet>
#include <QPair>
#include <QMultiMap>

#include <QFontMetrics>
#include <QGraphicsProxyWidget>

#include "control.h"
#include "disposer.h"
#include "../element.h"

#include "../../kernel/ids.h"

class Item;
class Disposer;
class EmbeddedControls : public QGraphicsItem {
	public:
		QRectF boundingRect() const;
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

		static bool existsInstance(Element *element);
		static void deleteInstance(Element *element);
		static void createInstance(Element *element, const qReal::EditorManager &editorManager);

	private:
		EmbeddedControls(
			Element *element,
			const qReal::EditorManager &editorManager
		);

		void disposeWidgets();
		void registerControls(Element *element);
		void initializeItems(Element *element, const qReal::EditorManager &editorManager);

		/* Data */
		Element *element;
		QList<Item> items;
		Disposer *disposer;
		QRectF computatedBoundingRect;

		/* Static */	//todo: следует сделать отдельным объектом
		static QSet<qReal::Id> forbiddenTypes;
		static QMultiMap<qReal::Id, QString> forbiddenProperties;
		static QMap<Element*, EmbeddedControls*> elementsToControls;

		static void loadSettings();
		static bool checkPermission(Element *element);

		/* Assist */	//todo: следует перенести куда-нибудь
		static qReal::Id extractElementId(const qReal::Id &id);
};

/* Measurable pair of control* and proxy* */

class Item : public QPair<Control*, QGraphicsProxyWidget*> {
	public:
		Item(Control* first, QGraphicsProxyWidget* second) :
			QPair<Control*, QGraphicsProxyWidget*>(first, second) {}

		qreal length() const {
			return textLength() + boundingLength();
		}
		qreal textLength() const {
			const Control* control = first;
			const QGraphicsProxyWidget* widget = second;
			QFontMetrics metrics(widget->font());
			return metrics.width(control->getPropertyName());
		}
		qreal boundingLength() const {
			const QGraphicsProxyWidget* widget = second;
			const QRectF bounding= widget->boundingRect();
			return bounding.right() - bounding.left();
		}
		bool operator<(const Item that) {
			return this->boundingLength() < that.boundingLength();
			//todo: когда будет текст, нужно будет вызывать length()
		}
};
