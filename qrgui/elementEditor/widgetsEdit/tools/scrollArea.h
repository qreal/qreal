#pragma once

#include <QtGui/QScrollArea>

#include "abstractScrollArea.h"

namespace qReal
{
namespace widgetsEdit
{

int const SCROLL_AREA_DEFAULT_WIDTH = 150;
int const SCROLL_AREA_DEFAULT_HEIGHT = 150;

class ScrollArea : public AbstractScrollArea
{
	Q_OBJECT

	Q_PROPERTY(bool widgetResizable READ widgetResizable WRITE setWidgetResizable USER true DESIGNABLE true)

public:
	ScrollArea(ToolController *controller);

private:
	bool widgetResizable() const;
	void setWidgetResizable(bool resizable);

	QScrollArea *mScrollArea;
};

}
}
