#pragma once

#include "plugins/editorPluginInterface/labelInterface.h"
#include "plugins/editorPluginInterface/labelFactoryInterface.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {

class LabelFactory : public LabelFactoryInterface
{
public:
	LabelFactory(models::GraphicalModelAssistApi &graphicalModelAssistApi, Id const &elementId);
	LabelInterface *createLabel(int index, qreal x, qreal y, QString const &text, qreal rotation);
	LabelInterface *createLabel(int index, qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);

private:
	models::GraphicalModelAssistApi &mGraphicalModelAssistApi;
	Id const mId;
};

}
