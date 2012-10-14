#pragma once

#include <QtCore/QDebug>
#include <QtGui/QWidget>

#include "rootWidget.h"
#include "layoutTool.h"
#include "../../../umllib/sdfRenderer.h"

namespace qReal
{
namespace widgetsEdit
{

int const ROOT_WIDGET_DEFAULT_WIDTH = 200;
int const ROOT_WIDGET_DEFAULT_HEIGHT = 200;

class Root : public LayoutTool
{
	Q_OBJECT

	Q_PROPERTY(QString shapeXml READ shapeXml WRITE setShapeXml USER true DESIGNABLE false)

public:
	Root(ToolController *controller);

	void setShape(QDomDocument const &shape);
	QDomDocument shapeDocument() const;

private:
	QString shapeXml() const;
	void setShapeXml(QString const &shape);

	RootWidget *mWidget;
	QList<Tool *> mItems;
};

}
}
