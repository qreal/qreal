#pragma once

#include "../editorPluginInterface/labelInterface.h"
#include "../editorPluginInterface/labelFactoryInterface.h"
#include "../models/graphicalModelAssistApi.h"

namespace qReal {

class LabelFactory : public LabelFactoryInterface
{
public:
	LabelFactory(models::GraphicalModelAssistApi &graphicalModelAssistApi);
	LabelInterface *createLabel(int index, qreal x, qreal y, QString const &text, qreal rotation);
	LabelInterface *createLabel(int index, qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);

private:
	models::GraphicalModelAssistApi &mGraphicalModelAssistApi;
};

}
