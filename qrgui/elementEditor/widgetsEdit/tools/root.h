#pragma once

#include <QtCore/QDebug>
#include <QtWidgets/QWidget>

#include "rootWidget.h"
#include "layoutTool.h"
#include "../../../umllib/sdfRenderer.h"

namespace qReal
{
namespace widgetsEdit
{

int const ROOT_WIDGET_DEFAULT_WIDTH = 200;
int const ROOT_WIDGET_DEFAULT_HEIGHT = 200;

class RootProxy : public LayoutToolProxy
{
	Q_OBJECT

	Q_PROPERTY(QString shapeXml READ shapeXml WRITE setShapeXml USER true DESIGNABLE false)

public:
	explicit RootProxy(RootWidget *widget);

	QString shapeXml() const;
	void setShapeXml(QString const &shape);

signals:
	void shapeXmlChanged(QString const &shape);

private:
	RootWidget *mWidget;
};

class Root : public LayoutTool
{
	Q_OBJECT

public:
	explicit Root(ToolController *controller);

	virtual void onLoaded();
	QDomDocument shapeDocument() const;

private:
	RootWidget *mWidget;
};

}
}
