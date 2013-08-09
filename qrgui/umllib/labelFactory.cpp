#include "labelFactory.h"
#include "label.h"

using namespace qReal;

LabelInterface *LabelFactory::createLabel(qreal x, qreal y, QString const &text, qreal rotation)
{
	return new Label(x, y, text, rotation);
}

LabelInterface *LabelFactory::createLabel(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation)
{
	return new Label(x, y, binding, readOnly, rotation);
}
