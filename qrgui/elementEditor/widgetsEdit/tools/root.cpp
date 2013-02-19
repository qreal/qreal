#include <QtGui/QGroupBox>

#include "root.h"

using namespace qReal::widgetsEdit;

Root::Root(ToolController *controller)
	: LayoutTool(new RootWidget, controller)
{
	mWidget = dynamic_cast<RootWidget *>(widget());
	setMovable(false);
	mTitle = tr("Root");
	mTag = "Root";
	mProxy = new RootProxy(mWidget);
}

void Root::onLoaded()
{
	LayoutTool::onLoaded();
	setGeometry(QRectF(0, 0
		, ROOT_WIDGET_DEFAULT_WIDTH
		, ROOT_WIDGET_DEFAULT_HEIGHT));
}

QDomDocument Root::shapeDocument() const
{
	return mWidget->shapeDocument();
}

RootProxy::RootProxy(RootWidget *widget)
	: LayoutToolProxy(widget), mWidget(widget)
{
}

QString RootProxy::shapeXml() const
{
	return mWidget->shapeDocument().toString(4);
}

void RootProxy::setShapeXml(QString const &shape)
{
	QDomDocument shapeDoc;
	if (!shape.isEmpty() && !shapeDoc.setContent(shape)) {
		qDebug() << "Could not load shape property for root";
		return;
	}
	if (shapeDoc.isNull()) {
		mWidget->setShape(shapeDoc);
		return;
	}
	QDomElement const docElem = shapeDoc.documentElement();
	int const width = docElem.attribute("sizex").toInt();
	int const height = docElem.attribute("sizey").toInt();
	if (width > 0 && height > 0) {
		mWidget->resize(width, height);
	}
	mWidget->setShape(shapeDoc);
}
