/* Copyright 2007-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	: Type(false, diagram)
	, mVisible(false)
	, mWidth(-1)
	, mHeight(-1)
	, mCreateChildrenFromMenu(false)
	, mResolving(false)
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
		type->mLabels.append(label->clone());
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

QStringList GraphicType::immediateParents() const
{
	QStringList result;
	for (const GeneralizationProperties &generalization : mParents) {
		result << generalization.name;
	}

	return result;
}

void GraphicType::generateCommonData(OutFile &out) const
{
	generateName(out);
	generateFriendlyName(out);
	generateDiagram(out);
	generateDescription(out);
	generateLabels(out);
}

void GraphicType::generateLabels(OutFile &out) const
{
	for (const Label *label : mLabels) {
		label->generateCodeForConstructor(out);
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
		return initRoles() && initParents() && initProperties() && initDividability() && initContainers()
				&& initGraphics() && initLabels() && initPossibleEdges() && initPortTypes()
				&& initCreateChildrenFromMenu() && initContainerProperties()
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
			qWarning() << "ERROR: anonymous parent of node" << qualifiedName();
			return false;
		}

		const QString overrides = parentElement.attribute("overrides");

		for (const auto &parent : mParents) {
			if (parent.name == parentName) {
				qWarning() << "ERROR: parent of node" << qualifiedName() << "duplicated";
				return false;
			}
		}

		mParents.append({parentName, overrides});
	}

	return true;
}

bool GraphicType::initProperties()
{
	initRoleProperties();

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
		if (!addProperty(property, "")) {
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

		if (beginName.isEmpty() || endName.isEmpty() || ((temp != "true") && (temp != "false"))) {
			qWarning() << "Error: one of attributes is incorrect " <<
				"(perhaps, \"beginName\" or \"endName\" is empty or " <<
				"\"directed\" isn't \"true\" or \"false\".')" << qualifiedName();
			return false;
		}

		const bool directed = temp == "true";
		const QString edgeName = NameNormalizer::normalize(qualifiedName());
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

bool GraphicType::addProperty(Property *property, const QString &roleName)
{
	QString propertyName = this->propertyName(property, roleName);
	if (propertyName.isEmpty()) {
		propertyName = property->name();
	}

	if (mProperties.contains(propertyName)) {
		// This will automaticly dispose property in this branch.
		QScopedPointer<Property> propertyDisposer(property);
		Q_UNUSED(propertyDisposer)
		/// @todo Good for overriding parent properties, but bad in multiple inheritance case
		/// --- we can allow invalid rhomb inheritance.
		if (mProperties[propertyName] != property && *mProperties[propertyName] != *property) {
			qWarning() << "Property" << propertyName << "duplicated with different attributes";
			return false;
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
		// Parents are searched in "native" context of a type, so if it was imported links must not be broken.
		const QString qualifiedParentName = generalization.name.contains("::")
				? generalization.name
				: nativeContext() + "::" + generalization.name;

		Type *parent = mDiagram->findType(qualifiedParentName);
		if (parent == nullptr) {
			// Parent was not found in local context, trying to search in global context
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
			if (!addProperty(property->clone(), "")) {
				return false;
			}
		}

		GraphicType * const graphicParent = dynamic_cast<GraphicType*>(parent);
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
		}
	}

	for (int i = 0; i < mLabels.size(); ++i) {
		mLabels.value(i)->changeIndex(i + 1);
	}

	mResolvingFinished = true;
	return true;
}

void GraphicType::generateName(OutFile &out) const
{
	const QString normalizedName = NameNormalizer::normalize(qualifiedName());
	out() << QString("\t\t\tsetName(\"%1\");\n").arg(normalizedName);

	/// @todo: I don`t know why we check it here but think it can live for now. This should be moved
	/// into appropriate place later.
	for (const QPair<QString, QStringList> &part : mDiagram->paletteGroups()) {
		for (auto part2 : part.second) {
			if (part2 == normalizedName && mAbstract == "true" ) {
				qDebug() << "ERROR! Element" << qualifiedName() << "is abstract.";
				return;
			}
		}
	}
}

void GraphicType::generateFriendlyName(OutFile &out) const
{
	const QString actualDisplayedName = displayedName().isEmpty() ? name() : displayedName();
	out() << QString("\t\t\tsetFriendlyName(QObject::tr(\"%1\"));\n").arg(actualDisplayedName);
}

void GraphicType::generateDiagram(OutFile &out) const
{
	const QString diagramName = NameNormalizer::normalize(mDiagram->name());
	out() << QString("\t\t\tsetDiagram(\"%1\");\n").arg(diagramName);
}

void GraphicType::generateDescription(OutFile &out) const
{
	out() << "\t\t\tsetDescription(QObject::tr(\"" << mDescription << "\"));\n";
}

void GraphicType::generatePropertyData(OutFile &out) const
{
	out() << "\t\tvoid initProperties()\n\t\t{\n";
	const auto keys = mProperties.keys();
	for (const QString &key : keys) {
		Property *property = mProperties[key];

		// Validating property names.
		if (property->name() == "fromPort" || property->name() == "toPort"
				|| property->name() == "from" || property->name() == "to"
				|| property->name() == "name")
		{
			qWarning() << "ERROR: predefined property" << property->name() << "should not appear in metamodel, ignored";
			continue;
		}

		const QString name = key == property->name() ? property->name() : key;
		const QString stringConstructor = property->type() == "string" ? "QObject::tr" : "QString::fromUtf8";
		out() << QString("\t\t\taddProperty(\"%1\", \"%2\", %3(\"%4\"), QObject::tr(\"%5\"), "\
				"QObject::tr(\"%6\"), %7);\n").arg(name, property->type(), stringConstructor
						, property->defaultValue(), property->displayedName(), property->description()
						, property->isReferenceProperty() ? "true" : "false");
	}
	out() << "\t\t}\n";
}

QString GraphicType::resourceName(const QString &resourceType) const
{
	QString name = NameNormalizer::normalize(qualifiedName());
	return name + resourceType + ".sdf";
}

QStringList GraphicType::containedTypes() const
{
	return mContains;
}

const QMap<QString, QPair<bool, bool> > &GraphicType::explosions() const
{
	return mExplosions;
}

QString GraphicType::boolToString(bool value) const
{
	return value ? "true" : "false";
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
