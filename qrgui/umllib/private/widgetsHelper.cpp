#include "widgetsHelper.h"
#include "../../elementEditor/widgetsEdit/widgetsEditor.h"
#include "../../../qrutils/xmlUtils.h"

WidgetsHelper::WidgetsHelper(NodeElement *element)
	: mElement(element), mWidget(NULL)
{
}

bool WidgetsHelper::initWidget(QString const &filename)
{
	QDomDocument const document = utils::xmlUtils::loadDocument(filename);
	if (document.isNull()) {
		return false;
	}
	mWidget = qReal::widgetsEdit::WidgetsEditor::deserializeWidget(document);
	if (!mWidget) {
		return false;
	}
	mElement->setWidget(mWidget);
	return true;
}
