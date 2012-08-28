#pragma once

#include <QtCore/QDebug>
#include <QtGui/QWidget>

#include "rootWidget.h"
#include "layoutTool.h"
#include "../../../umllib/sdfRenderer.h"

namespace Ui
{
namespace WidgetsEdit
{

int const ROOT_WIDGET_DEFAULT_WIDTH = 200;
int const ROOT_WIDGET_DEFAULT_HEIGHT = 200;

class Root : public LayoutTool
{

public:
	Root(ToolController *controller);

	void setShape(QDomDocument const &shape);
	QDomDocument shapeDocument() const;

private:
	RootWidget *mWidget;
	QList<Tool *> mItems;
};

}
}
