#include <QtCore/QRect>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>

#include "nodeElementWidgetsHelper.h"
#include "../../../qrutils/xmlUtils.h"
#include "../elementEditor/widgetsEdit/widgetsEditor.h"

using namespace umlLib::details;

NodeElementWidgetsHelper::NodeElementWidgetsHelper()
{
	initWidget();
}

void NodeElementWidgetsHelper::setNodeElement(NodeElement *element)
{
	mElement = element;
//	placeProxyWidget();
}

void NodeElementWidgetsHelper::onResized()
{
}

void NodeElementWidgetsHelper::initWidget()
{
	//This is for debug
	QString loadPath = "/home/dvvrd/dev/qreal/widgets/test.wtf";

	QDomDocument document = utils::xmlUtils::loadDocument(loadPath);
	if (document.isNull()) {
		mWidget = NULL;
		return;
	}
	mWidget = qReal::widgetsEdit::WidgetsEditor::deserializeWidget(document);
}

void NodeElementWidgetsHelper::placeProxyWidget()
{
	QWidget *widget = new QWidget;
	widget->setAttribute(Qt::WA_OpaquePaintEvent);

	QCheckBox *box = new QCheckBox("check box");
	QGridLayout *layout = new QGridLayout;
	layout->setMargin(0);
	layout->addWidget(box);
	widget->setLayout(layout);

	mElement->setWidget(widget);
}
