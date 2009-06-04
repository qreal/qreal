#include "non_graph_type.h"

#include <QDebug>

bool NumericType::init(QDomElement &xml_element)
{
	qDebug() << "Stub!";
	return true;
}

bool StringType::init(QDomElement &xml_element)
{
	qDebug() << "Stub!";
	return false;
}

bool EnumType::init(QDomElement &xml_element)
{
	QDomElement child;
	QString name = xml_element.attribute("id");
	QString qname = xml_element.attribute("name");

	if (name == "" || qname == "")
	{
		qDebug() << "anonymous enum found";
		return false;
	}
	values << qname; // WTF???
	for (child = xml_element.firstChildElement("value"); !child.isNull();
	     child = child.nextSiblingElement("value"))
		values << child.text();
	return true;
}
