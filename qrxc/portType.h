#pragma once

#include "nonGraphicType.h"

class PortType : public NonGraphicType
{
public:
	virtual bool init(QDomElement const &element, QString const &context);
	virtual Type *clone() const;

	virtual void generateCode(utils::OutFile &out);

	virtual bool generateEnumValues(utils::OutFile &out, bool isNotFirst);
	virtual void generatePropertyTypes(utils::OutFile &out);
	virtual void generatePropertyDefaults(utils::OutFile &out);
};

