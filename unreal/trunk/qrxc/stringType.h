#pragma once

#include "nonGraphicType.h"

#include <QDomElement>

class StringType : public NonGraphicType
{
public:
	virtual bool init(QDomElement const &element);
	virtual Type* clone() const;

private:
	QString mRegularExpression;
};
