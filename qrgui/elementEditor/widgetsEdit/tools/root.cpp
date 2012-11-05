#include <QtGui/QGroupBox>

#include "root.h"

using namespace qReal::widgetsEdit;

Root::Root(ToolController *controller)
	: LayoutTool(new RootWidget, controller)
{
	mWidget = dynamic_cast<RootWidget *>(widget());
	setGeometry(QRectF(0, 0
		, ROOT_WIDGET_DEFAULT_WIDTH
		, ROOT_WIDGET_DEFAULT_HEIGHT));
	setMovable(false);
	mTitle = "Root";
}

void Root::setShape(const QDomDocument &shape)
{
	if (shape.isNull()) {
		mWidget->setShape(shape);
		return;
	}
	QDomElement const docElem = shape.documentElement();
	int const width = docElem.attribute("sizex").toInt();
	int const height = docElem.attribute("sizey").toInt();
	resize(width, height);
	mWidget->setShape(shape);
}

QDomDocument Root::shapeDocument() const
{
	return mWidget->shapeDocument();
}

QString Root::shapeXml() const
{
	return mWidget->shapeDocument().toString(4);
}

void Root::setShapeXml(QString const &shape)
{
	QDomDocument shapeDoc;
	if (!shape.isEmpty() && !shapeDoc.setContent(shape)) {
		qDebug() << "Could not load shape property for root";
		return;
	}
	setShape(shapeDoc);
}
