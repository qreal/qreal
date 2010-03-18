#include "type.h"

namespace qRealType {
	qRealType::qRealType(QREAL_METATYPE type_)
	{
		type = type_;
		defaultValue = new qRealValue(this);

		switch (type)
		{
		case INTEGER:
			defaultValue->fromInteger(0);
			name = "Integer";
			break;
		case REAL:
			defaultValue->fromReal(0.0);
			name = "Real";
			break;
		case BOOLEAN:
			defaultValue->fromBoolean(false);
			name = "Boolean";
			break;
		case STRING:
			defaultValue->fromString("");
			name = "String";
			break;
		default:
			throw "Invalid metatype";
			break;
		}
	}

	qRealType::~qRealType()
	{
		delete defaultValue;
	}

	QString qRealType::toString()
	{
		return name;
	}

	qRealValue& qRealType::New()
	{
		return defaultValue->clone();
	}
}
