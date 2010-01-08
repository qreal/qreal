#pragma once

#include "nonGraphicType.h"

#include <QDomElement>
#include <QStringList>

class EnumType : public NonGraphicType
{
public:
	virtual bool init(QDomElement const &element, QString const &context);
	virtual Type* clone() const;

private:
	QStringList mValues;
};
