#include "graphicType.h"
#include "property.h"
#include "label.h"
#include "diagram.h"
#include "nameNormalizer.h"
#include "../qrutils/outFile.h"

#include <QDebug>

using namespace utils;

GraphicType::ContainerProperties::ContainerProperties() : isSortingContainer(false), sizeOfForestalling(0),
	sizeOfChildrenForestalling(0), hasMovableChildren(true), minimizesToChildren(false), maximizesChildren(false)
{}

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
	type->mContainerProperties = mContainerProperties;
	type->mContains = mContains;
}

bool GraphicType::init(QDomElement const &element, QString const &context)
{
	mElement = element;
	if (Type::init(element, context))
	{
		mDescription = element.attribute("description", "");
		mLogic = element.firstChildElement("logic");
		if (mLogic.isNull())
		{
			qDebug() << "ERROR: can't find logic tag of graphic type";
			return false;
		}
		mGraphics = element.firstChildElement("graphics");
        return initParents() && initProperties() initDissectability() && initContainers() && initAssociations()
			&& initGraphics() && initLabels() && initConnections() && initUsages() && initPossibleEdges()
			&& initContainerProperties() && initBonusContextMenuFields();
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

bool GraphicType::initFieldList(QString const &listName, QString const &listElementName
	, QStringList &resultingList, QString const &fieldName, bool const isNeedToNormalizeAtt) const
{
	QDomElement containerElement = mLogic.firstChildElement(listName);
	if (containerElement.isNull())
		return true;

	for (QDomElement childElement = containerElement.firstChildElement(listElementName);
		!childElement.isNull();
		childElement = childElement.nextSiblingElement(listElementName))
	{
		QString typeName;
		if (isNeedToNormalizeAtt)
			typeName = NameNormalizer::normalize(childElement.attribute(fieldName));
		else
			typeName = childElement.attribute(fieldName);

		if (typeName == "") {
			qDebug() << "Error: anonymous " << fieldName << " in the " << listName << " list, in " << qualifiedName();
			return false;
		}

		if (!resultingList.contains(typeName))
			resultingList.append(typeName);
		else {
			qDebug() << "ERROR: " << fieldName << " in the " << listName << " list in " << qualifiedName() << "duplicated";
			return false;
		}
	}
	return true;
}

bool GraphicType::initTypeList(QString const &listName, QString const &listElementName
	, QStringList &resultingList) const
{
	return initFieldList(listName, listElementName, resultingList, "type", true);
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

bool GraphicType::initBonusContextMenuFields()
{
	return initFieldList("bonusContextMenuFields", "field", mBonusContextMenuFields, "name", false);
}

bool GraphicType::initContainerProperties()
{
	QDomElement containerElement = mLogic.firstChildElement("container");
	if (containerElement.isNull())
		return true;

	QDomElement containerPropertiesElement = containerElement.firstChildElement("properties");
	if (containerPropertiesElement.isNull())
		return true;

	for (QDomElement childElement = containerPropertiesElement.firstChildElement();
		!childElement.isNull();
		childElement = childElement.nextSiblingElement())
	{
		if (childElement.tagName() == "sortContainer") {
			mContainerProperties.isSortingContainer = true;
		} else if (childElement.tagName() == "forestalling") {
			QString sizeAttribute = childElement.attribute("size");
			bool isSizeOk = false;
			mContainerProperties.sizeOfForestalling = sizeAttribute.toInt(&isSizeOk);
			if (!isSizeOk)
				return false;
		} else if (childElement.tagName() == "childrenForestalling") {
			QString sizeAttribute = childElement.attribute("size");
			bool isSizeOk = false;
			mContainerProperties.sizeOfChildrenForestalling = sizeAttribute.toInt(&isSizeOk);
			if (!isSizeOk)
				return false;
		} else if (childElement.tagName() == "minimizeToChildren") {
			mContainerProperties.minimizesToChildren = true;
		} else if (childElement.tagName() == "banChildrenMove") {
			mContainerProperties.hasMovableChildren = false;
		} else if (childElement.tagName() == "maximizeChildren") {
			mContainerProperties.maximizesChildren = true;
		}

	}

	return true;
}

bool GraphicType::initPossibleEdges()
{
		QString const listName = "possibleEdges";
		QString const listElementName = "possibleEdge";

		QDomElement containerElement = mLogic.firstChildElement(listName);
		if (containerElement.isNull())
				return true;

		for (QDomElement childElement = containerElement.firstChildElement(listElementName);
				!childElement.isNull();
				childElement = childElement.nextSiblingElement(listElementName))
		{
				QString beginName = NameNormalizer::normalize(childElement.attribute("beginName"));
				QString endName = NameNormalizer::normalize(childElement.attribute("endName"));
				QString temp = childElement.attribute("directed");
				bool directed = false;

				if (beginName.isEmpty() || endName.isEmpty() || ((temp != "true") && (temp != "false"))) {

						qDebug() << beginName;
						qDebug() << endName;
						qDebug() << temp;
						qDebug() << "Error: one of attributes is incorrect " <<
							"(perhaps, \"beginName\" or \"emptyName\" is empty or " <<
							"\"directed\" isn't \"true\" or \"false\".')" << qualifiedName();
						return false;
				}
				if (temp == "true")
					directed = true;

				QString edgeName = NameNormalizer::normalize(qualifiedName());
				QPair<QPair<QString,QString>,QPair<bool,QString> > possibleEdge(qMakePair(beginName,endName),qMakePair(directed,edgeName));

				if (!mPossibleEdges.contains(possibleEdge))
						mPossibleEdges.append(possibleEdge);
				else {
						qDebug() << "ERROR: this edge is already in list " << qualifiedName();
						return false;
				}
		}
		return true;
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
		// Pугаемся мы только тогда, когда тип, значение по умолчанию или что-то ещё
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
		QString qualifiedParentName = parentName.contains("::") ? parentName : nativeContext() + "::" + parentName;

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

		GraphicType* gParent = dynamic_cast<GraphicType*>(parent);
		if (gParent)
			foreach (PossibleEdge pEdge,gParent->mPossibleEdges) {
				mPossibleEdges.append(qMakePair(pEdge.first,qMakePair(pEdge.second.first,name())));
			}
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
		out() << "\telementsNameMap[\"" << diagramName << "\"][\"" << normalizedName << "\"] = QString::fromUtf8(\"" << actualDisplayedName << "\");\n";
	}
}

void GraphicType::generateDescriptionMapping(OutFile &out)
{
	if (mVisible) {
		if (!mDescription.isEmpty()) {
			QString diagramName = NameNormalizer::normalize(mDiagram->name());
			QString normalizedName = NameNormalizer::normalize(qualifiedName());
			out() << "\telementsDescriptionMap[\"" << diagramName << "\"][\"" << normalizedName << "\"] = QString::fromUtf8(\"" << mDescription << "\");\n";
		}
	}
}

void GraphicType::generatePropertyDescriptionMapping(utils::OutFile &out)
{
	if (mVisible) {
		QString const diagramName = NameNormalizer::normalize(mDiagram->name());
		QString const normalizedName = NameNormalizer::normalize(qualifiedName());
		foreach (Property const *p, mProperties) {
			if (!p->description().isEmpty()) {
				QString const propertyName = p->name();
				QString const propertyDescription = p->description();
				out() << "\tpropertiesDescriptionMap[\"" << diagramName << "\"][\""
						<< normalizedName << "\"][\"" << propertyName << "\"] = QString::fromUtf8(\"" << propertyDescription << "\");\n";
			}
		}
	}
}

void GraphicType::generatePropertyDisplayedNamesMapping(utils::OutFile &out)
{
	if (mVisible) {
		QString const diagramName = NameNormalizer::normalize(mDiagram->name());
		QString const normalizedName = NameNormalizer::normalize(qualifiedName());
		foreach (Property const *p, mProperties) {
			if (!p->displayedName().isEmpty()) {
				QString const propertyName = p->name();
				QString const propertyDisplayedName = p->displayedName();
				out() << "\tpropertiesDisplayedNamesMap[\"" << diagramName << "\"][\""
						<< normalizedName << "\"][\"" << propertyName << "\"] = QString::fromUtf8(\"" << propertyDisplayedName << "\");\n";
			}
		}
	}
}

void GraphicType::generateMouseGesturesMap(OutFile &out)
{
	if (mVisible) {
		QString pathStr = path();
		QString output = "";
		if (pathStr.isEmpty())
			return;

		output =  "\telementMouseGesturesMap[\"" + NameNormalizer::normalize(mDiagram->name()) + "\"][\"" +
											NameNormalizer::normalize(qualifiedName()) + "\"] = ";
		out() << output;

		if (pathStr.length() > maxLineLength - output.length()) {
			out() << "\"" << pathStr.left(maxLineLength - output.length());
			pathStr.remove(0, maxLineLength - output.length());
			QString prefix = "\t\t\t\"";
			do {
				out() << "\"\n" << prefix << pathStr.left(maxLineLength);
				pathStr.remove(0, maxLineLength);
			} while (pathStr.length() > maxLineLength);

			if (pathStr.length() > 0)
				out() << "\"\n" << prefix << pathStr;

		} else
			out() << "\"" << pathStr;
		out() << "\";\n";
	}
}

bool GraphicType::generateObjectRequestString(OutFile &out, bool isNotFirst)
{
	if (mVisible) {
		QString name = NameNormalizer::normalize(qualifiedName());
		generateOneCase(out, isNotFirst);
		out() << "\t\treturn new " << name << "();\n";
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
			if (propertiesString.length() >= maxLineLength) {
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

void GraphicType::generatePropertyTypes(OutFile &out)
{
	if (!mVisible)
		return;

	QString name = NameNormalizer::normalize(qualifiedName());

	foreach (Property *property, mProperties) {
		// skipping basic types since we're not really interested in them
		// TODO: lolwut?
		if (property->type() == "string" || property->name() == "int")
			continue;
		out() << "\tpropertyTypes[\"" << name << "\"][\"" << property->name() << "\"] = \"" << NameNormalizer::normalize(property->type()) << "\";\n";
	}
}

void GraphicType::generatePropertyDefaults(OutFile &out)
{
	if (!mVisible)
		return;

	QString name = NameNormalizer::normalize(qualifiedName());
	foreach (Property *property, mProperties)
		if (!property->defaultValue().isEmpty())
			out() << "\tpropertyDefault[\"" << name << "\"][\"" << property->name()
					<< "\"] = QString::fromUtf8(\"" << property->defaultValue() << "\");\n";
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

bool GraphicType::generatePossibleEdges(OutFile &out, bool isNotFirst)
{
	if (mPossibleEdges.isEmpty())
		return false;
	generateOneCase(out, isNotFirst);

	out() << "\t\tresult";
	foreach (PossibleEdge element, mPossibleEdges) {
		QString directed = "false";
		if (element.second.first)
			directed = "true";
		out() << " << qMakePair(qMakePair(QString(\"" << element.first.first << "\"),QString(\"" << element.first.second << "\")),"
				<< "qMakePair(" << directed << ",QString(\"" << element.second.second << "\")))";
	}
	out() << ";\n";
	return true;
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

void GraphicType::generateParentsMapping(utils::OutFile &out)
{
	if (mParents.isEmpty())
		return;

	QString diagramName = NameNormalizer::normalize(mDiagram->name());
	QString normalizedName = NameNormalizer::normalize(qualifiedName());
	out() << "\tparentsMap[\"" << diagramName << "\"][\"" << normalizedName << "\"]\n";
	foreach (QString const &parent, mParents) {
		out() << "\t\t<< qMakePair(QString(\"" << diagramName << "\"), QString(\"" << NameNormalizer::normalize(parent) << "\"))\n";
	}
	out() << "\t;\n";
}
