#include "graphicType.h"

#include <QtCore/QDebug>

#include <qrutils/outFile.h>

#include "property.h"
#include "label.h"
#include "diagram.h"
#include "nameNormalizer.h"
#include "nodeType.h"
#include "edgeType.h"

using namespace utils;

GraphicType::ContainerProperties::ContainerProperties()
		: isSortingContainer(false), sizeOfForestalling(4, 0)
		, sizeOfChildrenForestalling(0), hasMovableChildren(true)
		, minimizesToChildren(false), maximizesChildren(false)
{
}

GraphicType::GeneralizationProperties::GeneralizationProperties(const QString &name, const QString &overrides)
		: name(name)
{
	overridePorts = overrides.contains("ports", Qt::CaseInsensitive);
	overrideLabels = overrides.contains("labels", Qt::CaseInsensitive);
	overridePictures = overrides.contains("pictures", Qt::CaseInsensitive);
	if (overrides.contains("all", Qt::CaseInsensitive)) {
		overridePorts = true;
		overrideLabels = true;
		overridePictures = true;
	}
}

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
{
}

GraphicType::~GraphicType()
{
	foreach (Label *label, mLabels) {
		delete label;
	}
}

void GraphicType::copyFields(GraphicType *type) const
{
	Type::copyFields(type);
	type->mElement = mElement;
	type->mGraphics = mGraphics;
	type->mHeight = mHeight;
	for (Label *label : mLabels) {
		type->mLabels.append(new Label(*label));
	}

	type->mLogic = mLogic;
	type->mParents = mParents;
	type->mVisible = mVisible;
	type->mWidth = mWidth;
	type->mContainerProperties = mContainerProperties;
	type->mContains = mContains;
	type->mExplosions = mExplosions;
}

void GraphicType::copyLabels(GraphicType *parent)
{
	for (Label *label : parent->mLabels) {
		mLabels.append(label->clone());
	}
}

bool GraphicType::init(const QDomElement &element, const QString &context)
{
	mElement = element;
	if (Type::init(element, context)) {
		mDescription = element.attribute("description", "");
		mAbstract = element.attribute("abstract", "");
		mLogic = element.firstChildElement("logic");
		if (mLogic.isNull()) {
			qDebug() << "ERROR: can't find logic tag of graphic type";
			return false;
		}

		mGraphics = element.firstChildElement("graphics");
		return initParents() && initProperties() && initDividability() && initContainers() && initAssociations()
				&& initGraphics() && initLabels() && initPossibleEdges() && initPortTypes()
				&& initCreateChildrenFromMenu() && initContainerProperties() && initBonusContextMenuFields()
				&& initExplosions();
	}

	return false;
}

bool GraphicType::initParents()
{
	QDomElement parentsElement = mLogic.firstChildElement("generalizations");
	if (parentsElement.isNull()) {
		return true;
	}

	for (QDomElement parentElement = parentsElement.firstChildElement("parent")
			; !parentElement.isNull()
			; parentElement = parentElement.nextSiblingElement("parent"))
	{
		const QString parentName = parentElement.attribute("parentName");
		if (parentName.isEmpty()) {
			qDebug() << "Error: anonymous parent of node" << qualifiedName();
			return false;
		}

		const QString overrides = parentElement.attribute("overrides");

		for (const auto &parent : mParents) {
			if (parent.name == parentName) {
				qDebug() << "ERROR: parent of node" << qualifiedName() << "duplicated";
				return false;
			}
		}

		mParents.append({parentName, overrides});
	}

	return true;
}

bool GraphicType::initProperties()
{
	const QDomElement propertiesElement = mLogic.firstChildElement("properties");
	if (propertiesElement.isNull()) {
		return true;
	}
	for (QDomElement propertyElement = propertiesElement.firstChildElement("property")
			; !propertyElement.isNull()
			; propertyElement = propertyElement.nextSiblingElement("property"))
	{
		Property *property = new Property();
		if (!property->init(propertyElement)) {
			delete property;
			continue;
		}
		if (!addProperty(property)) {
			return false;
		}
	}
	return true;
}

bool GraphicType::initFieldList(const QString &listName, const QString &listElementName
	, QStringList &resultingList, const QString &fieldName, const bool isNeedToNormalizeAtt) const
{
	QDomElement containerElement = mLogic.firstChildElement(listName);
	if (containerElement.isNull()) {
		return true;
	}

	for (QDomElement childElement = containerElement.firstChildElement(listElementName)
			; !childElement.isNull()
			; childElement = childElement.nextSiblingElement(listElementName))
	{
		QString typeName;
		if (isNeedToNormalizeAtt) {
			typeName = NameNormalizer::normalize(childElement.attribute(fieldName));
		} else {
			typeName = childElement.attribute(fieldName);
		}

		if (typeName.isEmpty()) {
			qDebug() << "Error: anonymous " << fieldName << " in the " << listName << " list, in " << qualifiedName();
			return false;
		}

		if (!resultingList.contains(typeName)) {
			resultingList.append(typeName);
		} else {
			qDebug() << "ERROR: " << fieldName << " in the " << listName << " list in "
					 << qualifiedName() << "duplicated";
			return false;
		}
	}

	return true;
}

bool GraphicType::initTypeList(const QString &listName, const QString &listElementName
		, QStringList &resultingList) const
{
	return initFieldList(listName, listElementName, resultingList, "type", true);
}

bool GraphicType::initContainers()
{
	return initTypeList("container", "contains", mContains);
}

bool GraphicType::initBonusContextMenuFields()
{
	return initFieldList("bonusContextMenuFields", "field", mBonusContextMenuFields, "name", false);
}

bool GraphicType::initContainerProperties()
{
	QDomElement containerElement = mLogic.firstChildElement("container");
	if (containerElement.isNull()) {
		return true;
	}

	QDomElement containerPropertiesElement = containerElement.firstChildElement("properties");
	if (containerPropertiesElement.isNull()) {
		return true;
	}

	for (QDomElement childElement = containerPropertiesElement.firstChildElement()
			; !childElement.isNull()
			; childElement = childElement.nextSiblingElement())
	{
		if (childElement.tagName() == "sortContainer") {
			mContainerProperties.isSortingContainer = true;
		} else if (childElement.tagName() == "forestalling") {
			QString sizeAttribute = childElement.attribute("size");
			bool isSizeOk = false;
			mContainerProperties.sizeOfForestalling = toIntVector(sizeAttribute, &isSizeOk);
			if (!isSizeOk) {
				return false;
			}
		} else if (childElement.tagName() == "childrenForestalling") {
			QString sizeAttribute = childElement.attribute("size");
			bool isSizeOk = false;
			mContainerProperties.sizeOfChildrenForestalling = sizeAttribute.toInt(&isSizeOk);
			if (!isSizeOk) {
				return false;
			}
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

bool GraphicType::initCreateChildrenFromMenu()
{
	if (!mLogic.elementsByTagName("createChildrenFromMenu").isEmpty()) {
		mCreateChildrenFromMenu = true;
	}

	return true;
}

bool GraphicType::initPossibleEdges()
{
	const QString listName = "possibleEdges";
	const QString listElementName = "possibleEdge";

	QDomElement containerElement = mLogic.firstChildElement(listName);
	if (containerElement.isNull()) {
		return true;
	}

	for (QDomElement childElement = containerElement.firstChildElement(listElementName);
			!childElement.isNull();
			childElement = childElement.nextSiblingElement(listElementName))
	{
		QString beginName = NameNormalizer::normalize(childElement.attribute("beginName"));
		QString endName = NameNormalizer::normalize(childElement.attribute("endName"));
		QString temp = childElement.attribute("directed");
		bool directed = false;

		if (beginName.isEmpty() || endName.isEmpty()
				|| ((temp != "true") && (temp != "false")))
		{
			qDebug() << beginName;
			qDebug() << endName;
			qDebug() << temp;
			qDebug() << "Error: one of attributes is incorrect " <<
				"(perhaps, \"beginName\" or \"emptyName\" is empty or " <<
				"\"directed\" isn't \"true\" or \"false\".')" << qualifiedName();
			return false;
		}
		if (temp == "true") {
			directed = true;
		}

		QString edgeName = NameNormalizer::normalize(qualifiedName());
		QPair<QPair<QString, QString>, QPair<bool, QString> > possibleEdge(qMakePair(beginName, endName)
				, qMakePair(directed, edgeName));

		if (!mPossibleEdges.contains(possibleEdge)) {
			mPossibleEdges.append(possibleEdge);
		} else {
			qDebug() << "ERROR: this edge is already in list " << qualifiedName();
			return false;
		}
	}
	return true;
}

bool GraphicType::initExplosions()
{
	const QDomElement explodesTo = mLogic.firstChildElement("explodesTo");
	if (explodesTo.isNull()) {
		return true;
	}
	for (QDomElement targetElement = explodesTo.firstChildElement()
			; !targetElement.isNull()
			; targetElement = targetElement.nextSiblingElement())
	{
		const QString targetName = targetElement.attribute("type");
		if (targetName.isEmpty()) {
			return false;
		}
		const bool isReusable = targetElement.attribute("makeReusable", "false").toLower().trimmed() == "true";
		const bool immediateLinkage
				= targetElement.attribute("requireImmediateLinkage", "false").toLower().trimmed() == "true";
		mExplosions[targetName] = qMakePair(isReusable, immediateLinkage);
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
		if (!initLabel(label, element, count)) {
			delete label;
		} else {
			mLabels.append(label);
			++count;
		}
	}
	return true;
}

bool GraphicType::addProperty(Property *property)
{
	const QString propertyName = property->name();
	if (mProperties.contains(propertyName)) {
		/// @todo Good for overriding parent properties, but bad in multiple inheritance case
		/// --- we can allow invalid rhomb inheritance.
		if (mProperties[propertyName] != property && *mProperties[propertyName] != *property) {
			qDebug() << "WARNING: property" << propertyName << "duplicated with different attributes";
			delete property;
		}
	} else {
		mProperties[propertyName] = property;
	}
	return true;
}

bool GraphicType::isResolving() const
{
	return mResolving;
}

bool GraphicType::resolve()
{
	if (mResolvingFinished) {
		return true;
	}

	ResolvingHelper helper(mResolving);
	Q_UNUSED(helper)

	/// @todo Ensure that parents are not duplicated.

	for (const GeneralizationProperties &generalization : mParents) {
		// Предки ищутся в "родном" контексте типа, так что если он был импортирован, ссылки не должны поломаться.
		const QString qualifiedParentName = generalization.name.contains("::")
				? generalization.name
				: nativeContext() + "::" + generalization.name;

		Type *parent = mDiagram->findType(qualifiedParentName);
		if (parent == nullptr) {
			// В локальном контексте не нашлось, попробуем в глобальном
			parent = mDiagram->findType(generalization.name);
			if (parent == nullptr) {
				qDebug() << "ERROR: can't find parent" << generalization.name << "for" << qualifiedName();
				return false;
			}
		}

		if (parent->isResolving()) {
			qDebug() << "ERROR: circular inheritance between" << generalization.name << "and" << qualifiedName();
			return false;
		}

		if (!parent->isResolved()) {
			if (!parent->resolve()) {
				return false;
			}
		}

		for (Property *property : parent->properties().values()) {
			if (!addProperty(property->clone())) {
				return false;
			}
		}

		GraphicType* const graphicParent = dynamic_cast<GraphicType*>(parent);
		if (graphicParent != nullptr) {
			if (!generalization.overrideLabels) {
				copyLabels(graphicParent);
			}

			if (!generalization.overridePictures) {
				copyPictures(graphicParent);
			}

			NodeType* const nodeParent = dynamic_cast<NodeType*>(parent);
			if (nodeParent != nullptr) {
				if (!generalization.overridePorts) {
					copyPorts(nodeParent);
				}
			}

			for (PossibleEdge pEdge : graphicParent->mPossibleEdges) {
				mPossibleEdges.append(qMakePair(pEdge.first,qMakePair(pEdge.second.first,name())));
			}

			for (const QString &element : graphicParent->mExplosions.keys()) {
				if (!mExplosions.contains(element)) {
					mExplosions[element] = graphicParent->mExplosions[element];
				}
			}
		}
	}

	for (int i = 0; i < mLabels.size(); ++i) {
		mLabels.value(i)->changeIndex(i + 1);
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
		for (QPair<QString, QStringList> part : mDiagram->paletteGroups()) {
			for (auto part2 : part.second) {
				if (part2 == normalizedName && mAbstract == "true" ) {
					qDebug() << "ERROR! Element" << qualifiedName() << "is abstract.";
					return;
				}
			}
		}

		if (mAbstract == "true") {
			return;
		}

		out() << "\tmElementsNameMap[\"" << diagramName << "\"][\"" << normalizedName
				<< "\"] = tr(\"" << actualDisplayedName << "\");\n";
	}
}

void GraphicType::generateDescriptionMapping(OutFile &out)
{
	if (mVisible) {
		if (!mDescription.isEmpty()) {
			QString diagramName = NameNormalizer::normalize(mDiagram->name());
			QString normalizedName = NameNormalizer::normalize(qualifiedName());
			out() << "\tmElementsDescriptionMap[\"" << diagramName << "\"][\""
					<< normalizedName << "\"] = tr(\"" << mDescription << "\");\n";
		}
	}
}

void GraphicType::generatePropertyDescriptionMapping(utils::OutFile &out)
{
	if (mVisible) {
		const QString diagramName = NameNormalizer::normalize(mDiagram->name());
		const QString normalizedName = NameNormalizer::normalize(qualifiedName());
		foreach (const Property *p, mProperties) {
			if (!p->description().isEmpty()) {
				const QString propertyName = p->name();
				const QString propertyDescription = p->description();
				out() << "\tmPropertiesDescriptionMap[\"" << diagramName << "\"][\""
						<< normalizedName << "\"][\"" << propertyName << "\"] = tr(\""
						<< propertyDescription << "\");\n";
			}
		}
	}
}

void GraphicType::generatePropertyDisplayedNamesMapping(utils::OutFile &out)
{
	if (mVisible) {
		const QString diagramName = NameNormalizer::normalize(mDiagram->name());
		const QString normalizedName = NameNormalizer::normalize(qualifiedName());
		foreach (const Property *p, mProperties) {
			if (!p->displayedName().isEmpty()) {
				const QString propertyName = p->name();
				const QString propertyDisplayedName = p->displayedName();
				out() << "\tmPropertiesDisplayedNamesMap[\"" << diagramName << "\"][\""
						<< normalizedName << "\"][\"" << propertyName << "\"] = tr(\""
						<< propertyDisplayedName << "\");\n";
			}
		}
	}
}

void GraphicType::generateMouseGesturesMap(OutFile &out)
{
	if (mVisible) {
		QString pathStr = path();
		QString output = "";
		if (pathStr.isEmpty()) {
			return;
		}

		output =  "\tmElementMouseGesturesMap[\"" + NameNormalizer::normalize(mDiagram->name()) + "\"][\""
				+ NameNormalizer::normalize(qualifiedName()) + "\"] = ";
		out() << output;

		if (pathStr.length() > maxLineLength - output.length()) {
			out() << "\"" << pathStr.left(maxLineLength - output.length());
			pathStr.remove(0, maxLineLength - output.length());
			const QString prefix = "\t\t\t\"";
			do {
				out() << "\"\n" << prefix << pathStr.left(maxLineLength);
				pathStr.remove(0, maxLineLength);
			} while (pathStr.length() > maxLineLength);

			if (pathStr.length() > 0) {
				out() << "\"\n" << prefix << pathStr;
			}

		} else {
			out() << "\"" << pathStr;
		}
		out() << "\";\n";
	}
}

bool GraphicType::generateObjectRequestString(OutFile &out, bool isNotFirst)
{
	if (mVisible) {
		QString name = NameNormalizer::normalize(qualifiedName());
		generateOneCase(out, isNotFirst);
		out() << "\t\treturn new " << name << "();\n\t}\n";
		return true;
	}

	return false;
}

bool GraphicType::generateProperties(OutFile &out, bool isNotFirst, bool isReference)
{
	if (mVisible) {
		generateOneCase(out, isNotFirst);

		QString propertiesString;
		bool isFirstProperty = true;

		foreach (Property *property, mProperties) {
			// do not generate common properties
			if (property->name() == "fromPort" || property->name() == "toPort"
				|| property->name() == "from" || property->name() == "to"
				|| property->name() == "name")
			{
				qDebug() << "ERROR: predefined property" << property->name()
					<< "shall not appear in .xml, ignored";
				continue;
			}

			if (!isReference || property->isReferenceProperty()) {
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
		}

		if (!isFirstProperty) {
			out() << propertiesString << ";\n";
		}

		out() << "\t}\n";
		return true;
	}
	return false;
}

bool GraphicType::generatePorts(OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out)
	Q_UNUSED(isNotFirst)
	return false;
}

void GraphicType::generatePropertyTypes(OutFile &out)
{
	if (!mVisible) {
		return;
	}

	QString name = NameNormalizer::normalize(qualifiedName());

	foreach (Property *property, mProperties) {
		// skipping basic types since we're not really interested in them
		// TODO: lolwut?
		if (property->type() == "string" || property->name() == "int") {
			continue;
		}
		out() << "\tmPropertyTypes[\"" << name << "\"][\"" << property->name() << "\"] = \""
				<< NameNormalizer::normalize(property->type()) << "\";\n";
	}
}

void GraphicType::generatePropertyDefaults(OutFile &out)
{
	if (!mVisible) {
		return;
	}

	const QString name = NameNormalizer::normalize(qualifiedName());
	for (const Property *property : mProperties) {
		if (!property->defaultValue().isEmpty()) {
			const QString stringConstructor = property->type() == "string" ? "tr" : "QString::fromUtf8";
			out() << QString("\tmPropertyDefault[\"%1\"][\"%2\"] = %3(\"%4\");\n").arg(name
					, property->name(), stringConstructor, property->defaultValue());
		}
	}
}

void GraphicType::generateOneCase(OutFile &out, bool isNotFirst) const
{
	const QString name = NameNormalizer::normalize(qualifiedName());

	if (!isNotFirst) {
		out() << "\tif (element == \"" << name << "\") {\n";
	} else {
		out() << "\telse if (element == \"" << name << "\") {\n";
	}
}

QString GraphicType::resourceName(const QString &resourceType) const
{
	QString name = NameNormalizer::normalize(qualifiedName());
	return name + resourceType + ".sdf";
}

bool GraphicType::generateContainedTypes(OutFile &out, bool isNotFirst)
{
	return generateListForElement(out, isNotFirst, mContains);
}

bool GraphicType::generatePossibleEdges(OutFile &out, bool isNotFirst)
{
	if (mPossibleEdges.isEmpty()) {
		return false;
	}

	generateOneCase(out, isNotFirst);

	out() << "\t\tresult";
	foreach (PossibleEdge element, mPossibleEdges) {
		QString directed = "false";
		if (element.second.first) {
			directed = "true";
		}

		out() << " << qMakePair(qMakePair(QString(\"" << element.first.first << "\"),QString(\""
				<< element.first.second << "\")),"
				<< "qMakePair(" << directed << ",QString(\"" << element.second.second << "\")))";
	}
	out() << ";\n\t}\n";
	return true;
}

bool GraphicType::generateListForElement(utils::OutFile &out, bool isNotFirst, const QStringList &list) const
{
	if (list.isEmpty()) {
		return false;
	}

	generateOneCase(out, isNotFirst);

	out() << "\t\tresult ";
	foreach (const QString &element, list) {
		out() << "<< \"" << element << "\" ";
	}

	out() << ";\n\t}\n";
	return true;
}

void GraphicType::generateParentsMapping(utils::OutFile &out)
{
	if (mParents.isEmpty()) {
		return;
	}

	const QString diagramName = NameNormalizer::normalize(mDiagram->name());
	const QString normalizedName = NameNormalizer::normalize(qualifiedName());
	out() << "\tmParentsMap[\"" << diagramName << "\"][\"" << normalizedName << "\"]\n";
	for (const GeneralizationProperties &parent : mParents) {
		out() << "\t\t<< qMakePair(QString(\"" << diagramName << "\"), QString(\""
				<< NameNormalizer::normalize(parent.name) << "\"))\n";
	}

	out() << "\t;\n";
}

QVector<int> GraphicType::toIntVector(const QString &s, bool *isOk) const
{
	const QStringList strings = s.split(',');
	QVector<int> result(4, 0);

	if (strings.size() != 4) {
		*isOk = false;
		return result;
	}

	for (int i = 0; i < 4; i++) {
		result[i] = strings[i].toInt(isOk);
		if (!*isOk)
			return result;
	}

	return result;
}

void GraphicType::generateExplosionsMap(OutFile &out)
{
	if (mExplosions.isEmpty()) {
		return;
	}

	const QString diagramName = NameNormalizer::normalize(mDiagram->name());
	const QString normalizedName = NameNormalizer::normalize(qualifiedName());
	foreach (const QString &target, mExplosions.keys()) {
		const bool reusable = mExplosions[target].first;
		const bool immediateLinkage = mExplosions[target].second;

		out() << "\tmExplosionsMap[\"" << diagramName << "\"][\"" << normalizedName << "\"]";
		out() << QString(" << ExplosionData(\"%1\", \"%2\", %3, %4);\n").arg(diagramName
				, NameNormalizer::normalize(target)
				, reusable ? "true" : "false"
				, immediateLinkage ? "true" : "false");
	}
}
