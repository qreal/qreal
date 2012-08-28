#pragma once

#include <QtGui/QScrollArea>

#include "abstractScrollArea.h"

namespace Ui
{
namespace WidgetsEdit
{

int const SCROLL_AREA_DEFAULT_WIDTH = 150;
int const SCROLL_AREA_DEFAULT_HEIGHT = 150;

class ScrollArea : public AbstractScrollArea
{
public:
	ScrollArea(ToolController *controller);

private:
	QScrollArea *mScrollArea;
};

}
}
