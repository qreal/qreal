#include "tabWidget.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QTabBar>

using namespace qReal::gui;

TabWidget::TabWidget(QWidget *parent)
	: QTabWidget(parent)
{
}

void TabWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MiddleButton) {
		if (tabBar()->tabAt(event->pos()) > -1) {
			removeTab(tabBar()->tabAt(event->pos()));
		}
	}
}
