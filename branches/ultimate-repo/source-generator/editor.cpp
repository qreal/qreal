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
	QDomElement child = xml_element.firstChildElement();

	while (!child.isNull())
	{
		if (child.nodeName() == "enum")
		{
			if (!parseEnum(child))
				return false;
		}
		else if (child.nodeName() == "numeric")
		{
			if (!parseNumeric(child))
				return false;
		}
		else if (child.nodeName() == "string")
		{
			if (!parseString(child))
				return false;
		}
		else
			qDebug() << "WARNING: unknown non-graph metatype" << child.nodeName();
		child = child.nextSiblingElement();
	}

	return true;
}

bool Editor::parseGraphTypes(QDomElement &xml_element)
{
	QDomElement child = xml_element.firstChildElement();

	while (!child.isNull())
	{
		if (child.nodeName() == "node")
		{
			Node *node = new Node(this);
			if (!node->init(child))
			{
				delete node;
				return false;
			}
			objects << node;
		}
		else if (child.nodeName() == "edge")
		{
			Edge *edge = new Edge(this);
			if (!edge->init(child))
			{
				delete edge;
				return false;
			}
			objects << edge;
		}
		else
			qDebug() << "WARNING: unknown tag" << child.nodeName();
		child = child.nextSiblingElement();
	}
	return true;
}

bool Editor::init(QDomElement &xml_element)
{
	QDomElement child = xml_element.firstChildElement();

	qDebug() << "Processing" << name << "editor";
	while (!child.isNull())
	{
		if (child.nodeName() == "non_graph_types")
		{
			if (!parseNonGraphTypes(child)) return false;
		}
		else if (child.nodeName() == "graph_types")
		{
			if (!parseGraphTypes(child)) return false;
		}
		else
			qDebug() << "WARNING: unknown tag" << child.nodeName();
		child = child.nextSiblingElement();
	}
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
