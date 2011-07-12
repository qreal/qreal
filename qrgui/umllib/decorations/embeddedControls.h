#pragma once

#include <QPair>
#include <QFontMetrics>
#include <QGraphicsProxyWidget>

#include "control.h"
#include "../element.h"

class Item;
class EmbeddedControls;
typedef QMap<Element*, EmbeddedControls*> ElementsToControls;

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

		void   disposeWidgets();
		void   disposeWidget(QRectF &rect, qreal &curX, qreal &curY, int &i);
		QRectF disposeWidgets(const int columns, const qreal length, const qreal middleX, const qreal bottom);

		void initializeElement(Element *element);
		void registerControls(Element *element);
		void initializeItems(Element *element, const qReal::EditorManager &editorManager);

		bool isEdge;
		QRectF computatedBoundingRect;

		QList<Item> items;
		Element *element;
		static ElementsToControls elementsToControls;

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
			return metrics.width(control->getProperty());
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
