#pragma once

#include "nonGraphicType.h"

#include <QDomElement>

class StringType : public NonGraphicType
{
public:
	virtual bool init(QDomElement const &element, QString const &context);
	virtual Type* clone() const;
	virtual bool generateEnumValues(utils::OutFile &/*out*/, bool /*isNotFirst*/) { return false; }
	virtual void generatePropertyTypes(utils::OutFile &/*out*/) {}

private:
	QString mRegularExpression;
};
