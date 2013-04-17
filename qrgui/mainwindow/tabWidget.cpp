#include "tabWidget.h"

#include <QtGui/QMouseEvent>
#include <QtWidgets/QTabBar>

using namespace qReal::gui;

TabWidget::TabWidget(QWidget *parent)
	: QTabWidget(parent)
{
}

void TabWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MiddleButton && tabBar()->tabAt(event->pos()) > -1) {
		removeTab(tabBar()->tabAt(event->pos()));
	}
}

