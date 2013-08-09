#pragma once

#include "../editorPluginInterface/labelInterface.h"
#include "../editorPluginInterface/labelFactoryInterface.h"

namespace qReal {

class LabelFactory : public LabelFactoryInterface
{
public:
	LabelInterface *createLabel(qreal x, qreal y, QString const &text, qreal rotation);
	LabelInterface *createLabel(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation);
};

}
