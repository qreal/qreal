#pragma once

#include <QtGui/QScrollArea>

#include "abstractScrollArea.h"

namespace qReal
{
namespace widgetsEdit
{

int const SCROLL_AREA_DEFAULT_WIDTH = 150;
int const SCROLL_AREA_DEFAULT_HEIGHT = 150;

class ScrollAreaProxy : public AbstractScrollAreaProxy
{
	Q_OBJECT

	Q_PROPERTY(bool widgetResizable READ widgetResizable WRITE setWidgetResizable USER true DESIGNABLE true)

public:
	explicit ScrollAreaProxy(QScrollArea *scrollArea);

private:
	bool widgetResizable() const;
	void setWidgetResizable(bool resizable);

	QScrollArea *mScrollArea;
};

class ScrollArea : public AbstractScrollArea
{
	Q_OBJECT

public:
	explicit ScrollArea(ToolController *controller);

private:
	QScrollArea *mScrollArea;
};

}
}
