#include "numericType.h"

#include <QDebug>
#include "../qrutils/outFile.h"

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

	return false;
}

Type* NumericType::clone() const
{
	NumericType *result = new NumericType();
	Type::copyFields(result);
	result->mBaseType = mBaseType;
	return result;
}

bool NumericType::generateEnumValues(utils::OutFile &out, bool isNotFirst) 
{ 
	Q_UNUSED(out);
	Q_UNUSED(isNotFirst);
	return false; 
}

void NumericType::generatePropertyTypes(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void NumericType::generatePropertyDefaults(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void NumericType::generateMouseGesturesMap(utils::OutFile &out)
{
	Q_UNUSED(out);
}

