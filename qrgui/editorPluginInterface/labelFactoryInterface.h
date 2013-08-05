#pragma once

#include <QtCore/QString>

#include "labelInterface.h"

class LabelFactoryInterface
{
public:
	virtual LabelInterface *createTitle(qreal x, qreal y, QString const &text, qreal rotation) = 0;
	virtual LabelInterface *createTitle(qreal x, qreal y, QString const &binding, bool readOnly, qreal rotation) = 0;
};
