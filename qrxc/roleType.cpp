#include "roleType.h"

bool RoleType::init(const QDomElement &element, const QString &context)
{
	if (!NonGraphicType::init(element, context)) {
		return false;
	}

	mArrowType = element.attribute("arrowType");
	mEnd = element.attribute("end");
	mNavigable = element.attribute("navigable");

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
	result->mArrowType = mArrowType;
	result->mEnd = mEnd;
	result->mNavigable = mNavigable;
	result->mProperties = mProperties;
	Type::copyFields(result);
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

bool RoleType::isEnding()
{
	return mEnd == "true";
}

bool RoleType::isNavigable()
{
	return mNavigable == "true";
}


QString RoleType::typeOfArrow()
{
	return mArrowType;
}


QList<Property*> RoleType::getPropertiesOfRole()
{
	return mProperties;
}
