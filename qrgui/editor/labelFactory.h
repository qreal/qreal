#pragma once

#include "plugins/editorPluginInterface/labelInterface.h"
#include "plugins/editorPluginInterface/labelFactoryInterface.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {

class LabelFactory : public LabelFactoryInterface
{
public:
	LabelFactory(models::GraphicalModelAssistApi &graphicalModelAssistApi, const Id &elementId);
	LabelInterface *createLabel(int index, qreal x, qreal y, const QString &text, qreal rotation);
	LabelInterface *createLabel(int index, qreal x, qreal y, const QString &binding, bool readOnly, qreal rotation);

private:
	models::GraphicalModelAssistApi &mGraphicalModelAssistApi;
	const Id mId;
};

}
