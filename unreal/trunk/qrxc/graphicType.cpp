#include "graphicType.h"
#include "property.h"
#include "label.h"
#include "diagram.h"
#include "nameNormalizer.h"
#include "../utils/outFile.h"

#include <QDebug>

using namespace utils;

const int MAX_LINE_LENGTH = 60;

GraphicType::ResolvingHelper::ResolvingHelper(bool &resolvingFlag)
	: mResolvingFlag(resolvingFlag)
{
	mResolvingFlag = true;
}

GraphicType::ResolvingHelper::~ResolvingHelper()
{
	mResolvingFlag = false;
}

GraphicType::GraphicType(Diagram *diagram)
	: Type(false, diagram), mVisible(false), mWidth(-1), mHeight(-1), mResolving(false)
{}

GraphicType::~GraphicType()
{
	foreach (Label *label, mLabels)
		delete label;
}

void GraphicType::copyFields(GraphicType *type) const
{
	Type::copyFields(type);
	type->mElement = mElement;
	type->mGraphics = mGraphics;
	type->mHeight = mHeight;
	foreach (Label *label, mLabels)
		type->mLabels.append(new Label(*label));
	type->mLogic = mLogic;
	type->mParents = mParents;
	type->mVisible = mVisible;
	type->mWidth = mWidth;
}

bool GraphicType::init(QDomElement const &element, QString const &context)
{
	mElement = element;
	if (Type::init(element, context))
	{
		mLogic = element.firstChildElement("logic");
		if (mLogic.isNull())
		{
			qDebug() << "ERROR: can't find logic tag of graphic type";
			return false;
		}
		mGraphics = element.firstChildElement("graphics");
		return initParents() && initProperties() && initContainers() && initAssociations()
			&& initGraphics() && initLabels() && initConnections() && initUsages();
	}
	else
		return false;
}

bool GraphicType::initParents()
{
	QDomElement parentsElement = mLogic.firstChildElement("generalizations");
	if (parentsElement.isNull())
		return true;
	for (QDomElement parentElement = parentsElement.firstChildElement("parent");
		!parentElement.isNull();
		parentElement = parentElement.nextSiblingElement("parent"))
	{
		QString parentName = parentElement.attribute("parentName");
		if (parentName == "") {
			qDebug() << "Error: anonymous parent of node" << qualifiedName();
			return false;
		}

		if (!mParents.contains(parentName))
			mParents.append(parentName);
		else {
			qDebug() << "ERROR: parent of node" << qualifiedName() << "duplicated";
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
	for (QDomElement propertyElement = propertiesElement.firstChildElement("property");
		!propertyElement.isNull();
		propertyElement = propertyElement.nextSiblingElement("property"))
	{
		Property *property = new Property();
		if (!property->init(propertyElement))
		{
			delete property;
			continue;
		}
		if (!addProperty(property))
		{
			return false;
		}
	}
	return true;
}

bool GraphicType::initTypeList(QString const &listName, QString const &listElementName
	, QStringList &resultingList) const
{
	QDomElement containerElement = mLogic.firstChildElement(listName);
	if (containerElement.isNull())
		return true;

	for (QDomElement childElement = containerElement.firstChildElement(listElementName);
		!childElement.isNull();
		childElement = childElement.nextSiblingElement(listElementName))
	{
		QString typeName = NameNormalizer::normalize(childElement.attribute("type"));

		if (typeName == "") {
			qDebug() << "Error: anonymous type in the " << listName << " list, in " << qualifiedName();
			return false;
		}

		if (!resultingList.contains(typeName))
			resultingList.append(typeName);
		else {
			qDebug() << "ERROR: type in the " << listName << " list in " << qualifiedName() << "duplicated";
			return false;
		}
	}
	return true;
}

bool GraphicType::initContainers()
{
	return initTypeList("container", "contains", mContains);
}

bool GraphicType::initConnections()
{
	return initTypeList("connections", "connection", mConnections);
}

bool GraphicType::initUsages()
{
	return initTypeList("usages", "usage", mUsages);
}

bool GraphicType::initLabels()
{
	int count = 1;
	for (QDomElement element = mGraphics.firstChildElement("labels").firstChildElement("label");
		!element.isNull();
		element = element.nextSiblingElement("label"))
	{
		Label *label = new Label();
		if (!initLabel(label, element, count))
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
	if (mProperties.contains(propertyName)) {
		// Множественное наследование может приводить к тому, что одно свойство
		// может быть добавлено классу дважды (ромбовидное наследование, например).
		// Ругаемся мы только тогда, когда тип, значение по умолчанию или что-то ещё
		// у одноимённых свойств различны - тогда непонятно, что делать.
		if (mProperties[propertyName] != property
			&& *mProperties[propertyName] != *property)
		{
			qDebug() << "ERROR: property" << propertyName << "duplicated with different attributes";
			delete property;
			return false;
		}
	}
	else
		mProperties[propertyName] = property;
	return true;
}

bool GraphicType::isResolving() const
{
	return mResolving;
}

bool GraphicType::resolve()
{
	if (mResolvingFinished)
		return true;

	ResolvingHelper helper(mResolving);

	mParents.removeDuplicates();

	foreach (QString parentName, mParents) {
		// Предки ищутся в "родном" контексте типа, так что если он был импортирован, ссылки не должны поломаться.
		QString qualifiedParentName = nativeContext() + "::" + parentName;

		Type *parent = mDiagram->findType(qualifiedParentName);
		if (parent == NULL) {
			// В локальном контексте не нашлось, попробуем в глобальном
			parent = mDiagram->findType(parentName);
			if (parent == NULL) {
				qDebug() << "ERROR: can't find parent" << parentName << "for" << qualifiedName();
				return false;
			}
		}

		if (parent->isResolving()) {
			qDebug() << "ERROR: circular inheritance between" << parentName << "and" << qualifiedName();
			return false;
		}
		if (!parent->isResolved()) {
			if (!parent->resolve())
				return false;
		}
		foreach (Property *property, parent->properties().values())
			if (!addProperty(property->clone()))
				return false;
	}
	mResolvingFinished = true;
	return true;
}

void GraphicType::generateNameMapping(OutFile &out)
{
	if (mVisible) {
		QString diagramName = NameNormalizer::normalize(mDiagram->name());
		QString normalizedName = NameNormalizer::normalize(qualifiedName());
		QString actualDisplayedName = displayedName().isEmpty() ? name() : displayedName();
		out() << "\telementsNameMap[\"" << diagramName << "\"][\"" << normalizedName << "\"] = \"" << actualDisplayedName << "\";\n";
	}
}

bool GraphicType::generateObjectRequestString(OutFile &out, bool isNotFirst)
{
	if (mVisible) {
		QString name = NameNormalizer::normalize(qualifiedName());
		generateOneCase(out, isNotFirst);
		out() << "\t\treturn new UML::" << name << "();\n";
		return true;
	}
	return false;
}

bool GraphicType::generateProperties(OutFile &out, bool isNotFirst)
{
	if (mVisible) {
		generateOneCase(out, isNotFirst);

		QString propertiesString;
		bool isFirstProperty = true;

		foreach (Property *property, mProperties) {
			// Хак: не генерить предопределённые свойства, иначе они затрут
			// настоящие и линки будут цепляться к чему попало.
			if (property->name() == "fromPort" || property->name() == "toPort"
				|| property->name() == "from" || property->name() == "to"
				|| property->name() == "name")
			{
				qDebug() << "ERROR: predefined property" << property->name()
					<< "shall not appear in .xml, ignored";
				continue;
			}

			if (isFirstProperty) {
				out() << "\t\tresult ";
				isFirstProperty = false;
			}

			propertiesString += QString(" << \"" + property->name() + "\"");
			if (propertiesString.length() >= MAX_LINE_LENGTH) {
				out() << propertiesString;
				propertiesString = "\n\t\t";
			}
		}

		if (!propertiesString.trimmed().isEmpty())
			out() << propertiesString;

		out() << ";\n";
		return true;
	}
	return false;
}

void GraphicType::generateOneCase(OutFile &out, bool isNotFirst) const
{
	QString name = NameNormalizer::normalize(qualifiedName());

	if (!isNotFirst)
		out() << "\tif (element == \"" << name << "\")\n";
	else
		out() << "\telse if (element == \"" << name << "\")\n";
}

QString GraphicType::resourceName(QString const &resourceType) const
{
	QString name = NameNormalizer::normalize(qualifiedName());
	return name + resourceType + ".sdf";
}

bool GraphicType::generateContainedTypes(OutFile &out, bool isNotFirst)
{
	return generateListForElement(out, isNotFirst, mContains);
}

bool GraphicType::generateConnections(OutFile &out, bool isNotFirst)
{
	return generateListForElement(out, isNotFirst, mConnections);
}

bool GraphicType::generateUsages(OutFile &out, bool isNotFirst)
{
	return generateListForElement(out, isNotFirst, mUsages);
}

bool GraphicType::generateListForElement(utils::OutFile &out, bool isNotFirst, QStringList const &list) const
{
	if (list.isEmpty())
		return false;

	generateOneCase(out, isNotFirst);

	out() << "\t\tresult ";
	foreach (QString element, list)
		out() << "<< \"" << element << "\" ";

	out() << ";\n";
	return true;
}
