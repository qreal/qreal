#include "numericType.h"

#include <QDebug>

bool NumericType::init(QDomElement const &element, QString const &context)
{
	if (NonGraphicType::init(element, context)) {
		QString baseTypeName = element.firstChildElement("base_type").text();
		if (baseTypeName == "int")
			mBaseType = IntType;
		else if (baseTypeName == "float")
			mBaseType = FloatType;
		else {
			qDebug() << "ERROR: unknown numeric base type" << baseTypeName;
			return false;
		}
		return true;
	}
	else
		return false;
}

Type* NumericType::clone() const
{
	NumericType *result = new NumericType();
	Type::copyFields(result);
	result->mBaseType = mBaseType;
	return result;
}

