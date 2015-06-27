#include "widgetFinder.h"

using namespace utils;

QWidget *WidgetFinder::widget(QWidget *parent, const QString &type, const QString &name)
{
	const QList<QWidget *> widgetList = parent->findChildren<QWidget *>(name);

	for (QWidget * const widget : widgetList) {
		const char *typeName = type.toLocal8Bit().data();
		if (widget->inherits(typeName) && widget->isVisible()) {
			return widget;
		}
	}

	return nullptr;
}
