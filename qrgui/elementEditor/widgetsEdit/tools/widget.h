#pragma once

#include <QtGui/QWidget>

#include "layoutTool.h"

namespace Ui
{
namespace WidgetsEdit
{

int const WIDGET_DEFAULT_WIDTH = 150;
int const WIDGET_DEFAULT_HEIGHT = 150;

class Widget : public LayoutTool
{
public:
	Widget(ToolController *controller);

private:
	QWidget *mWidget;
};

}
}
