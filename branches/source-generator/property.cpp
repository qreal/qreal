#include <QDomDocument>
#include <QDomNode>
#include "entity.h"
#include "property.h"

bool RealProperty::init(QDomElement &xml_element)
{
	QDomNamedNodeMap attrs = xml_element.attributes();

	// Fetch property name
	name = attrs.namedItem("name").toAttr().value();
	if (name == "") name = xml_element.firstChildElement("name").text();
	if (name == "")
	{
		qDebug() << "Anonymous prop of " << entity->id << " entity";
		return false;
	}

	// Fetch property type
	type = attrs.namedItem("type").toAttr().value();
	if (type == "") type = xml_element.firstChildElement("type").text();
	if (type == "")
	{
		qDebug() << "Unspecified type for prop" << name
		         << "of" << entity->id << "entity";
		return false;
	}

	// Sanity checks for all available types
	if (type == "enum")
	{
		type = xml_element.firstChildElement("enum").attribute("idref");
		if (type == "")
		{
			qDebug() << "Anonymous enum type for prop" << name
			         << "of" << entity->id << "entity";
			return false;
		}
		// TODO: Lookup enum
		is_enum = true;
	}
	else if (type == "ref")
	{
		type = xml_element.firstChildElement("ref").attribute("idref");
	}
	else if (type != "int" && type != "string" && type != "bool" &&
	         type != "positiveInt" && type != "nonNegativeInt" && type != "text")
	{
		qDebug() << "Unknown type for prop " << name << " of "
		         << entity->id << " entity";
		return false;
	}

	// Even if do not exist, not an error
	description = xml_element.firstChildElement("description").text();
	default_value = xml_element.firstChildElement("default").text();

	return true;
}
