#include <QDomElement>
#include "editor_file.h"
#include "editor.h"

Editor::Editor(QString dname, EditorFile *file)
{
	efile = file;
	name = dname;
}

Editor::~Editor()
{
	while (!objects.isEmpty())
		delete objects.takeFirst();
}

bool Editor::parseEnum(QDomElement &xml_element)
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
	for (child = xml_element.firstChildElement("value"); !child.isNull();
	     child = child.nextSiblingElement("value"))
		values << child.text();
	enumerations.insert(name, values);
	return true;
}

bool Editor::parseNumeric(QDomElement &xml_element)
{
	// STUB
	return true;
}

bool Editor::parseString(QDomElement &xml_element)
{
	// STUB
	return true;
}

bool Editor::parseNonGraphTypes(QDomElement &xml_element)
{
	QDomElement child;

	// I. Parse enums
	for (child = xml_element.firstChildElement("enum"); !child.isNull();
	     child = child.nextSiblingElement("enum"))
		if (!parseEnum(child))
			return false;

	// II. Parse numeric types
	for (child = xml_element.firstChildElement("numeric"); !child.isNull();
	     child = child.nextSiblingElement("numeric"))
		if (!parseNumeric(child))
			return false;

	// III. Parse string types
	for (child = xml_element.firstChildElement("string"); !child.isNull();
	     child = child.nextSiblingElement("string"))
		if (!parseString(child))
			return false;
	return true;
}

bool Editor::parseGraphTypes(QDomElement &xml_element)
{
	QDomElement child;

	// I. Parse nodes
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

	// II. Parse edges
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

bool Editor::init(QDomElement &xml_element)
{
	QDomElement child;

	qDebug() << "Processing" << name << "editor";

	child = xml_element.firstChildElement("non_graph_types");
	if (!parseNonGraphTypes(child))
		return false;

	child = xml_element.firstChildElement("graph_types");
	if (!parseGraphTypes(child))
		return false;

	return true;
}

bool Editor::resolve(void)
{
	Entity *e;

	Q_FOREACH(e, objects)
		if (!e->resolve())
			return false;
	return true;
}

const Entity* Editor::findEntityInTree(QString id) const
{
	const Entity *res;

	// Search in this diagram of this file
	res = findEntity(id);
	if (!res)
	{
		// Search in include files
		res = efile->findEntityInIncludes(id);
	}
	return res;
}

const Entity* Editor::findEntity(QString id) const
{
	Entity *e;

	Q_FOREACH(e, objects)
		if (e->id == id)
			return e;
	return NULL;
}
