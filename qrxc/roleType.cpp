#include "roleType.h"

bool RoleType::init(const QDomElement &element, const QString &context)
{
	if (!NonGraphicType::init(element, context)) {
		return false;
	}

	mArrowType = element.attribute("arrowType");
	QDomElement first = element.firstChildElement("properties");

	for (QDomElement propertyElement = first.firstChildElement("property")
		 ; !propertyElement.isNull()
		 ; propertyElement = propertyElement.nextSiblingElement("property"))
	{
		Property *property = new Property();
		bool check = property->init(propertyElement);
		mProperties.append(property);
	}

	return true;
}


Type* RoleType::clone() const
{
	RoleType *result = new RoleType();
	Type::copyFields(result);
	result->mProperties = mProperties;
	return result;
}

bool RoleType::generateEnumValues(utils::OutFile &out, bool isNotFirst)
{
	return false;
}

void RoleType::generatePropertyTypes(utils::OutFile &out)
{

}

void RoleType::generatePropertyDefaults(utils::OutFile &out)
{

}

void RoleType::generateMouseGesturesMap(utils::OutFile &out)
{

}

void RoleType::generateExplosionsMap(utils::OutFile &out)
{

}

QString RoleType::typeOfArrow()
{
	return mArrowType;
}


QList<Property*> RoleType::getPropertiesOfRole()
{
	return mProperties;
}
