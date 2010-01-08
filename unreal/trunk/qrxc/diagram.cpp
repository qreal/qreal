#include "diagram.h"
#include "type.h"
#include "enumType.h"
#include "numericType.h"
#include "stringType.h"
#include "nodeType.h"
#include "edgeType.h"
#include "editor.h"

#include <QDebug>

Diagram::Diagram(QString const &name, Editor *editor)
	: mDiagramName(name), mEditor(editor)
{}

Diagram::~Diagram()
{
	foreach(Type *type, mTypes.values())
		if (type)
			delete type;
}

bool Diagram::init(QDomElement const &diagramElement)
{
	for (QDomElement element = diagramElement.firstChildElement(); !element.isNull();
		element = element.nextSiblingElement())
	{
		if (element.nodeName() == "graphicTypes") {
			if (!initGraphicTypes(element))
				return false;
		} else if (element.nodeName() == "nonGraphicTypes") {
			if (!initNonGraphicTypes(element))
				return false;
		} else
			qDebug() << "ERROR: unknown tag" << element.nodeName();
	}
	return true;
}

bool Diagram::initGraphicTypes(QDomElement const &graphicTypesElement)
{
	for (QDomElement element = graphicTypesElement.firstChildElement(); !element.isNull();
		element = element.nextSiblingElement())
	{
		if (element.nodeName() == "node") {
			Type *nodeType = new NodeType(this);
			if (!nodeType->init(element, mDiagramName)) {
				delete nodeType;
				qDebug() << "Can't parse node";
				return false;
			}
			mTypes[nodeType->qualifiedName()] = nodeType;
		} else if (element.nodeName() == "edge") {
			Type *edgeType = new EdgeType(this);
			if (!edgeType->init(element, mDiagramName)) {
				delete edgeType;
				qDebug() << "Can't parse edge";
				return false;
			}
			mTypes[edgeType->qualifiedName()] = edgeType;
		} else if (element.nodeName() == "import") {
			QString importedElementName = element.attribute("name", "");
			QString importAs = element.attribute("as", "");
			mImports.append(QPair<QString, QString>(importedElementName, importAs));
		}
		else
		{
			qDebug() << "ERROR: unknown graphic type" << element.nodeName();
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
			if (!enumType->init(element, mDiagramName))
			{
				delete enumType;
				qDebug() << "Can't parse enum";
				return false;
			}
			mTypes[enumType->qualifiedName()] = enumType;
		} else if (element.nodeName() == "numeric")
		{
			Type *numericType = new NumericType();
			if (!numericType->init(element, mDiagramName))
			{
				delete numericType;
				qDebug() << "Can't parse numeric type";
				return false;
			}
			mTypes[numericType->qualifiedName()] = numericType;
		} else if (element.nodeName() == "string")
		{
			Type *stringType = new StringType();
			if (!stringType->init(element, mDiagramName))
			{
				delete stringType;
				qDebug() << "Can't parse string type";
				return false;
			}
			mTypes[stringType->qualifiedName()] = stringType;
		}
		else
		{
			qDebug() << "ERROR: unknown non graphic type" << element.nodeName();
			return false;
		}
	}
	return true;
}

bool Diagram::resolve()
{
	typedef QPair<QString, QString> ImportPair;
	foreach (ImportPair import, mImports) {
		Type *importedType = mEditor->findType(import.first);
		if (importedType == NULL) {
			qDebug() << "ERROR: imported type" << import.first << "not found, skipping";
			continue;
		}
		Type *copiedType = importedType->clone();
		copiedType->setName(import.second);
		copiedType->setDiagram(this);
		copiedType->setContext(mDiagramName);
		mTypes.insert(copiedType->qualifiedName(), copiedType);
	}

	foreach(Type *type, mTypes.values())
		if (!type->resolve()) {
			qDebug() << "ERROR: can't resolve type" << type->name();
			return false;
		}

	return true;
}

Editor* Diagram::editor() const
{
	return mEditor;
}

Type* Diagram::findType(QString name)
{
	if (mTypes.contains(name))
		return mTypes[name];
	else
		return mEditor->findType(name);
}

QMap<QString, Type*> Diagram::types() const
{
	return mTypes;
}

QString Diagram::name() const
{
	return mDiagramName;
}
