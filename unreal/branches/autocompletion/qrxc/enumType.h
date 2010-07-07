#pragma once

#include "nonGraphicType.h"

#include <QDomElement>
#include <QStringList>
#include <QDebug>

namespace utils {
	class OutFile;
}

class EnumType : public NonGraphicType
{
public:
	virtual bool init(QDomElement const &element, QString const &context);
	virtual Type* clone() const;
	virtual bool generateEnumValues(utils::OutFile &out, bool isNotFirst);
	virtual void generatePropertyTypes(utils::OutFile &/*out*/) {}

private:
	void generateOneCase(utils::OutFile &out, bool isNotFirst) const;
	
	QStringList mValues;
};
