#include <QDomElement>
#include "editor_file.h"
#include "category.h"

Category::Category(QString dname, EditorFile *file)
{
	efile = file;
	name = dname;
}

Category::~Category()
{
}

bool Category::parseEnum(QDomElement &xml_element)
{
	QDomElement child;
	QStringList values;
	QString name = xml_element.attribute("id");
	QString qname = xml_element.attribute("name");

	if (name == "" || qname == "")
	{
		qDebug() << "anonymous enum found";
		return false;
	}
	values << qname; // WTF???
	for (child = xml_element.firstChildElement("enumValue"); !child.isNull();
	     child = child.nextSiblingElement("enumValue"))
		values << child.text();
	enumerations.insert(name, values);
	return true;
}

bool Category::init(QDomElement &xml_element)
{
	QDomElement child;

	qDebug() << "initializing" << name << "category";
	// I. Parse enums
	for (child = xml_element.firstChildElement("enumType"); !child.isNull();
	     child = child.nextSiblingElement("enumType"))
		if (!parseEnum(child))
			return false;

	// II. Parse nodes
	for (child = xml_element.firstChildElement("node"); !child.isNull();
	     child = child.nextSiblingElement("node"))
	{
		Node *node = new Node(this);
		if (!node->init(child))
		{
			delete node;
			return false;
		}
		objects << node;
	}

	// III. Parse edges
	for (child = xml_element.firstChildElement("edge"); !child.isNull();
	     child = child.nextSiblingElement("edge"))
	{
		Edge *edge = new Edge(this);
		if (!edge->init(child))
		{
			delete edge;
			return false;
		}
		objects << edge;
	}
	return true;
}

bool Category::resolve(void)
{
	FOR_ALL_OBJECTS(this, o)
		if (!(*o)->resolve())
			return false;
	return true;
}

const Entity* Category::findEntityInTree(QString id) const
{
	const Entity *res;
	QList<EditorFile*>::ConstIterator j;

	// Search in this diagram of this file
	res = findEntity(id);
	if (!res)
	{
		// Search in include files
		res = efile->findEntityInIncludes(id);
	}
	return res;
}

const Entity* Category::findEntity(QString id) const
{
	FOR_ALL_OBJECTS(this, o)
		if ((*o)->id == id)
			return (*o);
	return NULL;
}
