#include "labelFactory.h"

#include "editor/label.h"

using namespace qReal;

LabelFactory::LabelFactory(models::GraphicalModelAssistApi &graphicalModelAssistApi, const Id &elementId)
	: mGraphicalModelAssistApi(graphicalModelAssistApi)
	, mId(elementId)
{
}

LabelInterface *LabelFactory::createLabel(int index, qreal x, qreal y, QString const &text, qreal rotation)
{
	return new Label(mGraphicalModelAssistApi, mId, index, x, y, text, rotation);
}

LabelInterface *LabelFactory::createLabel(int index, qreal x, qreal y, QString const &binding, bool readOnly
		, qreal rotation)
{
	return new Label(mGraphicalModelAssistApi, mId, index, x, y, binding, readOnly, rotation);
}
