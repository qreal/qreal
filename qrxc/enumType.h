#pragma once

#include <QtXml/QDomElement>
#include <QtCore/QStringList>

#include "nonGraphicType.h"

namespace utils {
	class OutFile;
}

class EnumType : public NonGraphicType
{
public:
	virtual bool init(QDomElement const &element, QString const &context);
	virtual Type* clone() const;
	virtual bool generateEnumValues(utils::OutFile &out, bool isNotFirst);
	virtual void generatePropertyTypes(utils::OutFile &out);
	virtual void generatePropertyDefaults(utils::OutFile &out);
	virtual void generateMouseGesturesMap(utils::OutFile &out);
	virtual void generateExplosionsMap(utils::OutFile &out);

private:
	void generateOneCase(utils::OutFile &out, bool isNotFirst) const;

	QStringList mValues;
};
