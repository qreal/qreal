#include "diagram.h"
#include "type.h"
#include "enumType.h"
#include "numericType.h"
#include "stringType.h"
#include "nodeType.h"
#include "edgeType.h"
#include "editor.h"

#include <qDebug>

Diagram::Diagram(QString const &name, Editor *editor)
	: mDiagramName(name), mEditor(editor)
{}

Diagram::~Diagram()
{
	foreach(Type *type, mTypes.values())
	{
		if (type)
		{
			delete type;
		}
	}
}

bool Diagram::init(QDomElement const &diagramElement)
{
	for (QDomElement element = diagramElement.firstChildElement(); !element.isNull();
		element = element.nextSiblingElement())
	{
		if (element.nodeName() == "graphicTypes")
		{
			if (!initGraphicTypes(element)) 
			{
				return false;
			}
		}
		if (element.nodeName() == "nonGraphicTypes")
		{
			if (!initNonGraphicTypes(element))
			{
				return false;
			}
		}
		else
		{
			QString name = element.nodeName();
			if ((name != "graphicTypes") && (name != "nonGraphicTypes"))
			{
				qDebug() << "Error: unknown tag" << name;
			}
		}
	}
	return true;
}


bool Diagram::initGraphicTypes(QDomElement const &graphicTypesElement)
{
	for (QDomElement element = graphicTypesElement.firstChildElement(); !element.isNull();
		element = element.nextSiblingElement())
	{
		if (element.nodeName() == "node")
		{
			Type *nodeType = new NodeType(this);
			if (!nodeType->init(element))
			{
				delete nodeType;
				qDebug() << "Can't parse node";
				return false;
			}
			mTypes[nodeType->name()] = nodeType;
		}
		else if (element.nodeName() == "edge")
		{
			Type *edgeType = new EdgeType(this);
			if (!edgeType->init(element))
			{
				delete edgeType;
				qDebug() << "Can't parse edge";
				return false;
			}
			mTypes[edgeType->name()] = edgeType;
		}
		else
		{
			qDebug() << "Error: unknown graphic type" << element.nodeName();
			return false;
		}
	}
	return true;
}

bool Diagram::initNonGraphicTypes(QDomElement const &nonGraphicTypesElement)
{
	for (QDomElement element = nonGraphicTypesElement.firstChildElement(); !element.isNull();
		element = element.nextSiblingElement())
	{
		if (element.nodeName() == "enum")
		{
			Type *enumType = new EnumType();
			if (!enumType->init(element))
			{
				delete enumType;
				qDebug() << "Can't parse enum";
				return false;
			}
			mTypes[enumType->name()] = enumType;
		} else if (element.nodeName() == "numeric")
		{
			Type *numericType = new NumericType();
			if (!numericType->init(element))
			{
				delete numericType;
				qDebug() << "Can't parse numeric type";
				return false;
			}
			mTypes[numericType->name()] = numericType;
		} else if (element.nodeName() == "string")
		{
			Type *stringType = new StringType();
			if (!stringType->init(element))
			{
				delete stringType;
				qDebug() << "Can't parse string type";
				return false;
			}
			mTypes[stringType->name()] = stringType;
		}
		else
		{
			qDebug() << "Error: unknown non graphic type" << element.nodeName();
			return false;
		}
	}
	return true;
}

bool Diagram::resolve()
{
	foreach(Type *type, mTypes.values())
	{
		if (!type->resolve())
		{
			qDebug() << "Error: can't resolve";
			return false;
		}
	}
	return true;
}

Editor* Diagram::editor()
{
	return mEditor;
}

Type* Diagram::findType(QString name)
{
	if (mTypes.contains(name))
	{
		return mTypes[name];
	}
	else
	{
		return mEditor->findType(name);
	}
}

QMap<QString, Type*> Diagram::types()
{
	return mTypes;
}

QString Diagram::name()
{
	return mDiagramName;
}