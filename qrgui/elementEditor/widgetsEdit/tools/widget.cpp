#include "widget.h"

using namespace qReal::widgetsEdit;

Widget::Widget(ToolController *controller)
	: LayoutTool(new QWidget, controller)
{
	mWidget = dynamic_cast<QWidget *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/widget.png");
	mTitle = tr("Widget");
	mWidget->setGeometry(0, 0
		, WIDGET_DEFAULT_WIDTH
		, WIDGET_DEFAULT_HEIGHT);
	mWidget->setMinimumWidth(WIDGET_DEFAULT_MIN_WIDTH);
	mWidget->setMinimumHeight(WIDGET_DEFAULT_MIN_HEIGHT);
}
