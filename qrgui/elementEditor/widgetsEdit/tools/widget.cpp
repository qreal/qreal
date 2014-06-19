#include "widget.h"

using namespace qReal::widgetsEdit;

Widget::Widget(ToolController *controller)
	: LayoutTool(new QWidget, controller)
{
	mWidget = dynamic_cast<QWidget *>(widget());
	mTitle = tr("Widget");
	mTag = "Widget";
	mIcon = QIcon(":/icons/widgetsEditor/widget.png");
	mWidget->setGeometry(0, 0
		, WIDGET_DEFAULT_WIDTH
		, WIDGET_DEFAULT_HEIGHT);
	mWidget->setMinimumWidth(WIDGET_DEFAULT_MIN_WIDTH);
	mWidget->setMinimumHeight(WIDGET_DEFAULT_MIN_HEIGHT);
	mProxy = new WidgetProxy(mWidget);
}

WidgetProxy::WidgetProxy(QWidget *widget)
	: LayoutToolProxy(widget), mWidget(widget)
{
}
