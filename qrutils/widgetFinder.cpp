#include "widgetFinder.h"

using namespace utils;

QWidget *WidgetFinder::widget(QWidget *parent, QString const &type, QString const &name)
{
	QList<QWidget *> const widgetList = parent->findChildren<QWidget *>(name);

	for (QWidget * const widget : widgetList) {
		if (type == widget->metaObject()->classInfo() && widget->isVisible()) {
			return widget;
		}
	}

	return nullptr;
}
