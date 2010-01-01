#pragma once

#include "nonGraphicType.h"

#include <QDomElement>
#include <QStringList>

class EnumType : public NonGraphicType
{
public:
	virtual bool init(QDomElement const &element);

private:
	QStringList mValues;
};
