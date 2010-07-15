#pragma once

#include "nonGraphicType.h"

#include <QDomElement>

class StringType : public NonGraphicType
{
public:
	virtual bool init(QDomElement const &element, QString const &context);
	virtual Type* clone() const;
	virtual bool generateEnumValues(utils::OutFile &out, bool isNotFirst);
	virtual void generatePropertyTypes(utils::OutFile &out);
	virtual void generatePropertyDefaults(utils::OutFile &out); 
	virtual void generateMouseGesturesMap(utils::OutFile &out);

private:
	QString mRegularExpression;
};
