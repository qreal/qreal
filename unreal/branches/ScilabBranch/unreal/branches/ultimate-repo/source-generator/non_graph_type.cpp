#include "non_graph_type.h"

#include <QDebug>

bool NumericType::init(QDomElement &xml_element)
{
	QDomElement child;
	QString id = xml_element.attribute("id");
	name = xml_element.attribute("name");

	if (id == "" || name == "")
	{
		qDebug() << "anonymous numeric type found";
		return false;
	}
	child = xml_element.firstChildElement("base_type");
	QString base = child.text();
	if (base == "int")
	{
		base_type = QREAL_NG_NUMERIC_INT;
		// Read constraints.
	}
	else if (base == "float")
	{
		base_type = QREAL_NG_NUMERIC_FLOAT;
		// Read constraints.
	}
	else
	{
		qDebug() << "unknown numeric base type" << base;
		return false;
	}
	return true;
}

bool StringType::init(QDomElement &xml_element)
{
	QDomElement constr, child;
	QString id = xml_element.attribute("id");
	name = xml_element.attribute("name");

	if (id == "" || name == "")
	{
		qDebug() << "anonymous string type found";
		return false;
	}
	constr = xml_element.firstChildElement("constraints");
	child = constr.firstChildElement("regexp");
	regexp = child.text();
	return false;
}

bool EnumType::init(QDomElement &xml_element)
{
	QDomElement child;
	QString id = xml_element.attribute("id");
	name = xml_element.attribute("name");

	if (id == "" || name == "")
	{
		qDebug() << "anonymous enum found";
		return false;
	}
	values << name; // WTF???
	for (child = xml_element.firstChildElement("value"); !child.isNull();
	     child = child.nextSiblingElement("value"))
		values << child.text();
	return true;
}
