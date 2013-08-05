#pragma once

#include "../editorPluginInterface/labelInterface.h"
#include "../editorPluginInterface/labelFactoryInterface.h"

class LabelFactory : public LabelFactoryInterface
{
public:
	LabelInterface *createTitle(qreal x, qreal y, QString const &text, qreal rotation);
	LabelInterface *createTitle(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);
};
