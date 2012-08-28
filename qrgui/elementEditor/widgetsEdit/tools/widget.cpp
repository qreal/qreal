#include "widget.h"

using namespace Ui::WidgetsEdit;

Widget::Widget(ToolController *controller)
	: LayoutTool(new QWidget, controller)
{
	mWidget = dynamic_cast<QWidget *>(widget());
	mIcon = QIcon(":/icons/widgetsEditor/widget.png");
	mTitle = "Widget";
	mWidget->setGeometry(0, 0
		, WIDGET_DEFAULT_WIDTH
		, WIDGET_DEFAULT_HEIGHT);
}
