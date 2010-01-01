#pragma once

#include "nonGraphicType.h"

#include <QDomElement>

enum BaseType
{
	IntType,
	FloatType
};

class NumericType : public NonGraphicType
{
public:
	virtual bool init(QDomElement const &element);

private:
	BaseType mBaseType;
};
