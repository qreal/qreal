#include "graphicType.h"
#include "property.h"
#include "label.h"
#include "diagram.h"
#include "nameNormalizer.h"
#include "../utils/outFile.h"

#include <QDebug>

using namespace utils;

const int MAX_LINE_LENGTH = 60;

GraphicType::GraphicType(Diagram *diagram)
	: Type(false), mDiagram(diagram), mVisible(false), mWidth(-1), mHeight(-1)
{}

GraphicType::~GraphicType()
{
	foreach (Label *label, mLabels)
	{
		delete label;
	}
}

bool GraphicType::init(QDomElement const &element)
{
	mElement = element;
	if (Type::init(element))
	{
		mLogic = element.firstChildElement("logic");
		if (mLogic.isNull())
		{
			qDebug() << "ERROR: can't find logic tag of graphic type";
			return false;
		}
		addKernelParent();
		if (!initParents())
			return false;
		if (!initProperties())
			return false;
		if (!initAssociations())
			return false;
		if (!initGraphics())
			return false;
		if (!initLabels())
			return false;
		return true;
	}
	else
	{
		return false;
	}
}

bool GraphicType::initParents()
{
	QDomElement parentsElement = mLogic.firstChildElement("parents");
	if (parentsElement.isNull())
	{
		return true;
	}
	for (QDomElement parentElement = parentsElement.firstChildElement("parent"); !parentElement.isNull();
		parentElement = parentElement.nextSiblingElement("parent"))
	{
		QString parentName = parentElement.attribute("parentName");
		if (parentName == "")
		{
			qDebug() << "Error: anonymous parent of node";
			return false;
		}

		if (!mParents.contains(parentName))
			mParents.append(parentName);
		else
		{
			qDebug() << "ERROR: parent of node duplicated";
			return false;
		}
	}
	return true;
}

bool GraphicType::initProperties()
{
	QDomElement propertiesElement = mLogic.firstChildElement("properties");
	if (propertiesElement.isNull())
	{
		return true;
	}
	for (QDomElement propertyElement = propertiesElement.firstChildElement("property"); !propertyElement.isNull();
		propertyElement = propertyElement.nextSiblingElement("property"))
	{
		Property *property = new Property();
		if (!property->init(propertyElement))
		{
			delete property;
			return false;
		}
		if (!addProperty(property))
		{
			return false;
		}
	}
	return true;
}

bool GraphicType::initLabels()
{
	int count = 1;
	for (QDomElement element = mElement.firstChildElement("labels").firstChildElement("label"); !element.isNull();
		element = element.nextSiblingElement("label"))
	{
		Label *label = new Label();
		if (!label->init(element, count))
			delete label;
		else {
			mLabels.append(label);
			++count;
		}
	}
	return true;
}

bool GraphicType::addProperty(Property *property)
{
	QString propertyName = property->name();
	if (mProperties.contains(propertyName))
	{
		if (!(mProperties[propertyName] == property))
		{
			qDebug() << "ERROR: property duplicated";
			delete property;
			return false;
		}
	}
	else
	{
		mProperties[propertyName] = property;
	}
	return true;
}

bool GraphicType::resolve()
{
	if (mResolvingFinished)
	{
		return true;
	}

	foreach (QString parentName, mParents)
	{
		Type *parent = mDiagram->findType(parentName);
		if (parent == NULL)
		{
			qDebug() << "ERROR: can't find parent" << parentName;
			return false;
		}
		if (!parent->isResolved())
		{
			if (!parent->resolve())
			{
				return false;
			}
			return true;
		}
		foreach(Property *property, parent->properties().values())
		{
			if (!addProperty(property->clone()))
			{
				return false;
			}
		}
	}
	mResolvingFinished = true;
	return true;
}

void GraphicType::generateNameMapping(OutFile &out)
{
	if (mVisible)
	{
		QString diagramName = NameNormalizer::normalize(mDiagram->name());
		QString name = NameNormalizer::normalize(mName);
		out() << "\telementsNameMap[\"" << diagramName << "\"][\"" << name << "\"] = \"" << mName << "\";\n";
	}
}

bool GraphicType::generateObjectRequestString(OutFile &out, bool notIsFirst)
{
	if (mVisible)
	{
		QString name = NameNormalizer::normalize(mName);
		generateOneCase(out, notIsFirst);
		out() << "\t\treturn new UML::" << name << "();\n";
		return true;
	}
	return false;
}


bool GraphicType::generateProperties(OutFile &out, bool notIsFirst)
{
	if (mVisible)
	{
		generateOneCase(out, notIsFirst);

		QString propertiesString;
		bool isFirstProperty = true;

		foreach (Property *property, mProperties)
		{
			// Хак: не генерить предопределённые свойства, иначе они затрут
			// настоящие и линки будут цепляться к чему попало.
			if (property->name() == "fromPort" || property->name() == "toPort"
				|| property->name() == "from" || property->name() == "to"
				|| property->name() == "name")
			{
				continue;
			}

			if (isFirstProperty){
				out() << "\t\tresult ";
				isFirstProperty = false;
			}

			propertiesString += QString(" << \"" + property->name() + "\"");
			if( propertiesString.length() >= MAX_LINE_LENGTH )
			{
				out() << propertiesString;
				propertiesString = "\n\t\t";
			}
		}
		if(!propertiesString.trimmed().isEmpty())
		{
			out() << propertiesString;
		}
		out() << ";\n";
		return true;
	}
	return false;
}

void GraphicType::generateOneCase(OutFile &out, bool notIsFirst)
{
	QString name = NameNormalizer::normalize(mName);

	if (!notIsFirst)
	{
		out() << "\tif (element == \"" << name << "\")\n";
	}
	else
	{
		out() << "\telse if (element == \"" << name << "\")\n";
	}
}
