#include "labelFactory.h"
#include "label.h"

using namespace qReal;

LabelFactory::LabelFactory(models::GraphicalModelAssistApi &graphicalModelAssistApi)
	: mGraphicalModelAssistApi(graphicalModelAssistApi)
{
}

LabelInterface *LabelFactory::createLabel(int index, qreal x, qreal y, QString const &text, qreal rotation)
{
	return new Label(mGraphicalModelAssistApi, index, x, y, text, rotation);
}

LabelInterface *LabelFactory::createLabel(int index, qreal x, qreal y, QString const &binding, bool readOnly
		, qreal rotation)
{
	return new Label(mGraphicalModelAssistApi, index, x, y, binding, readOnly, rotation);
}
