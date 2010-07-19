#include "stringType.h"

#include <QDebug>

bool StringType::init(QDomElement const &element, QString const &context)
{
	if (NonGraphicType::init(element, context)) {
		mRegularExpression = element.firstChildElement("regularExpression").text();
		return true;
	}
	else
		return false;
}

Type* StringType::clone() const
{
	StringType *result = new StringType();
	Type::copyFields(result);
	result->mRegularExpression = mRegularExpression;
	return result;
}

bool StringType::generateEnumValues(utils::OutFile &out, bool isNotFirst) 
{ 
	Q_UNUSED(out);
	Q_UNUSED(isNotFirst);
	return false; 
}

void StringType::generatePropertyTypes(utils::OutFile &out) 
{
	Q_UNUSED(out);
}

void StringType::generatePropertyDefaults(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void StringType::generateMouseGesturesMap(utils::OutFile &out)
{
	Q_UNUSED(out);
}

