#pragma once

#include <QtXml/QDomElement>

#include "nonGraphicType.h"

class StringType : public NonGraphicType
{
public:
	virtual bool init(const QDomElement &element, const QString &context);
	virtual Type* clone() const;
	virtual bool generateEnumValues(utils::OutFile &out, bool isNotFirst);
	virtual void generatePropertyTypes(utils::OutFile &out);
	virtual void generatePropertyDefaults(utils::OutFile &out);
	virtual void generateMouseGesturesMap(utils::OutFile &out);
	virtual void generateExplosionsMap(utils::OutFile &out);

private:
	QString mRegularExpression;
};
