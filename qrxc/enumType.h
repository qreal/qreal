#pragma once

#include "nonGraphicType.h"

namespace utils {
	class OutFile;
}

class EnumType : public NonGraphicType
{
public:
	virtual bool init(const QDomElement &element, const QString &context);
	virtual Type* clone() const;
	virtual bool generateEnumValues(utils::OutFile &out, bool isNotFirst);
	virtual void generatePropertyTypes(utils::OutFile &out);
	virtual void generatePropertyDefaults(utils::OutFile &out);
	virtual void generateMouseGesturesMap(utils::OutFile &out);
	virtual void generateExplosionsMap(utils::OutFile &out);

	/// Returns true if enum was marked as editable in metamodel.
	bool isEditable() const;

private:
	void generateOneCase(utils::OutFile &out, bool isNotFirst) const;

	QMap<QString, QString> mValues;
	bool mIsEditable;
};
