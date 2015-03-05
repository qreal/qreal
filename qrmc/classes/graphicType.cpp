#include "graphicType.h"
#include "property.h"
#include "../diagram.h"
#include "../utils/nameNormalizer.h"

#include <QDebug>

using namespace qrmc;
using namespace qReal;

GraphicType::ContainerProperties::ContainerProperties() : isSortContainer(false), sizeOfForestalling(0),
	sizeOfChildrenForestalling(0), isChildrenMovable(true), isMinimizingToChildren(false), isClass(false), isMaximizingChildren(false)
{}

GraphicType::ResolvingHelper::ResolvingHelper(bool &resolvingFlag)
	: mResolvingFlag(resolvingFlag)
{
	mResolvingFlag = true;
}

GraphicType::GraphicType(Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id)
	: Type(false, diagram, api, id), mResolving(false)
{
}

GraphicType::~GraphicType()
{
}

bool GraphicType::init(QString const &context)
{
	Type::init(context);

	mIsVisible = false;
	if (mApi->hasProperty(mId, "shape"))
		mIsVisible = !mApi->stringProperty(mId, "shape").isEmpty();

	if (mApi->hasProperty(mId, "RequestBody"))
		mIsVisible = !mApi->stringProperty(mId, "RequestBody").isEmpty();

	IdList const outLinks = mApi->outgoingLinks(mId);
	foreach (Id const outLink, outLinks) {
		if (outLink.element() == "Container") {
			Id const elementId = mApi->to(outLink);
			QString const typeName = mApi->name(elementId);
			mContains << typeName.split(",", QString::SkipEmptyParts);
		} else if (outLink.element() == "Inheritance") {
			Id const elementId = mApi->to(outLink);
			QString const childName = mApi->name(elementId);
			if (!mChildren.contains(childName)) {
				mChildren << childName.split(",", QString::SkipEmptyParts);
			}
		}
	}

	IdList const inLinks = mApi->incomingLinks(mId);
	foreach (Id const inLink, inLinks) {
		if (inLink.element() == "Inheritance") {
			Id const elementId = mApi->from(inLink);
			QString const parentName = mApi->name(elementId);
			if (!mParents.contains(parentName)) {
				mParents << parentName.split(",", QString::SkipEmptyParts);
			}
		}
	}

	foreach(Id id, mApi->children(mId)) {
		if (!mApi->isLogicalElement(id))
			continue;

		if (id.element() == metaEntityAttribute) {
			Property *property = new Property(mApi, id);
			if (!property->init()) {
				delete property;
				continue;
			}
			if (!addProperty(property))
				return false;
		} else if (id.element() == metaEntityConnection) {
			mConnections << mApi->stringProperty(id, "type").section("::", -1);
		} else if (id.element() == metaEntityUsage) {
			mUsages << mApi->stringProperty(id, "type").section("::", -1);
		} else if (id.element() == metaEntityContextMenuField)
		{
			mContextMenuItems << mApi->name(id);
		}
	}
	initPossibleEdges();
	initShape();
	return true;
}

bool GraphicType::initPossibleEdges()
{
	IdList children = mApi->children(mId);
	foreach(Id id, children) {
		if (!mApi->isLogicalElement(id) || id.element() != metaEntityPossibleEdge)
			continue;

		QString beginName = mApi->stringProperty(id, "beginName");
		QString endName = mApi->stringProperty(id, "endName");
		QString directedField = mApi->stringProperty(id, "directed");
		bool directed = false;

		if (beginName.isEmpty() || endName.isEmpty() || ((directedField != "true") && (directedField != "false"))) {

				qDebug() << "Error: one of attributes is incorrect " <<
					"(perhaps, \"beginName\" or \"emptyName\" is empty or " <<
					"\"directed\" isn't \"true\" or \"false\".')" << qualifiedName();
				return false;
		}
		directed = (directedField == "true");
		QString edgeName = NameNormalizer::normalize(qualifiedName());
		QPair<QPair<QString,QString>,QPair<bool,QString> > possibleEdge(qMakePair(beginName,endName),qMakePair(directed,edgeName));

		if (!mPossibleEdges.contains(possibleEdge))
				mPossibleEdges.append(possibleEdge);
		else {
			// FIXME: ignoring for now
				//qDebug() << "ERROR: this edge is already in list " << qualifiedName();
//				return false;
		}
	}
	return true;
}

void GraphicType::initShape()
{
	if (mApi->hasProperty(mId, "shape")) {
		QString shape = mApi->stringProperty(mId, "shape");
		if (shape.isEmpty())
			return;
		mShape.init(shape);
	}
	return;
}

GraphicType::ResolvingHelper::~ResolvingHelper()
{
	mResolvingFlag = false;
}

void GraphicType::copyFields(GraphicType *type) const
{
	Type::copyFields(type);
	type->mParents = mParents;
	type->mIsVisible = mIsVisible;
	type->mContainerProperties = mContainerProperties;
	type->mContains = mContains;
	type->mShape = mShape; // this is bad, mkay?
}

bool GraphicType::resolve()
{
	if (mResolvingFinished)
		return true;

	ResolvingHelper helper(mResolving);
	mParents.removeDuplicates();
	foreach (QString parentName, mParents) {
		// searching for parents in native context. if it was imported, references will remain valid
		QString qualifiedParentName = parentName.contains("::") ? parentName : nativeContext() + "::" + parentName;

		Type *parent = mDiagram->findType(qualifiedParentName);
		if (parent == NULL) {
			// didn't find in local context, try global
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


	return true;
}

bool GraphicType::isResolving() const
{
	return mResolving;
}

bool GraphicType::addProperty(Property *property)
{
	QString propertyName = property->name();
	if (mProperties.contains(propertyName)) {
		if (mProperties[propertyName] != property && *mProperties[propertyName] != *property)
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

bool GraphicType::isGraphicalType() const
{
	return mIsVisible;
}

QString GraphicType::generateProperties(QString const &lineTemplate) const
{
	if (!mIsVisible)
		return "";
	QString propertiesString;
	foreach (Property *property, mProperties) {
		// hack: don't generate pre-defined properties
		if (property->name() == "fromPort" || property->name() == "toPort"
			|| property->name() == "from" || property->name() == "to"
			|| property->name() == "name")
		{
			qDebug() << "ERROR: predefined property" << property->name()
				<< "shall not appear in metamodels, ignored";
			continue;
		}
		QString tmp = property->generatePropertyLine(lineTemplate) + endline;
		propertiesString += tmp.replace(elementNameTag, name());
	}
	return propertiesString;
}

QString GraphicType::generatePropertyDefaults(QString const &lineTemplate) const
{
	if (!mIsVisible)
		return "";
	QString defaultsString;
	foreach (Property *property, mProperties) {
		QString tmp = property->generateDefaultsLine(lineTemplate);
		if (!tmp.isEmpty())
			defaultsString += tmp.replace(elementNameTag, name()) + endline;
	}
	return defaultsString;
}

QString GraphicType::generatePropertyDisplayedNames(QString const &lineTemplate) const
{
	if (!mIsVisible)
		return "";
	QString displayedNamesString;
	foreach (Property *property, mProperties) {
		QString tmp = property->generateDisplayedNameLine(lineTemplate);
		if (!tmp.isEmpty()) {
			displayedNamesString += tmp.replace(elementNameTag, name()).replace(diagramNameTag, mContext) + endline;
		}
	}
	return displayedNamesString;
}

QString GraphicType::generateReferenceProperties(QString const &lineTemplate) const
{
	if (!mIsVisible)
		return "";
	QString referencePropertiesString = lineTemplate;
	QString referencePropertiesList = "";
	foreach (Property const *const property, mProperties) {
		if (property->isReferenceProperty()) {
			referencePropertiesList = referencePropertiesList + " << "  + "\"" + property->name() + "\"";
		}
	}
	if (referencePropertiesList.isEmpty()) {
		return "";
	} else {
		referencePropertiesString.replace(referencePropertiesListTag, referencePropertiesList).replace(elementNameTag, name());
		return referencePropertiesString;
	}
}

QString GraphicType::generateParents(QString const &lineTemplate) const
{
	QString parentsMapString;
	QString const diagramName = mContext + "::";
	QString parentName = qualifiedName().remove(diagramName);
	foreach (QString const child, mChildren) {
		QString tmp = lineTemplate;
		parentsMapString += tmp.replace(parentNameTag, parentName).replace(childNameTag, child).replace(diagramNameTag, mContext) + endline;
	}
	return parentsMapString;
}

QString GraphicType::generateContainers(QString const &lineTemplate) const
{
	if (!isGraphicalType() || mContains.isEmpty())
		return "";

	QString containersList;
	QString line = lineTemplate;
	foreach(QString contains, mContains) {
		containersList += "<< \"" + contains + "\" ";
	}
	line.replace(containersListTag, containersList).replace(elementNameTag, name());
	return line;
}

QString GraphicType::generateConnections(QString const &lineTemplate) const
{
	if (!isGraphicalType() || mConnections.isEmpty())
		return "";
	QString connectionsList;
	QString line = lineTemplate;
	foreach(QString connection, mConnections) {
		connectionsList += "<< \"" + connection + "\" ";
	}
	line.replace(connectionsListTag, connectionsList).replace(elementNameTag, name());
	return line;
}

QString GraphicType::generateUsages(QString const &lineTemplate) const
{
	if (!isGraphicalType() || mUsages.isEmpty())
		return "";
	QString usagesList;
	QString line = lineTemplate;
	foreach(QString usage, mUsages) {
		usagesList += "<< \"" + usage + "\" ";
	}
	line.replace(usagesListTag, usagesList).replace(elementNameTag, name());
	return line;
}

QString GraphicType::generateEnums(QString const &lineTemplate) const
{
	Q_UNUSED(lineTemplate);
	return "";
}

QString GraphicType::generatePossibleEdges(QString const &lineTemplate) const
{
	if (mPossibleEdges.isEmpty())
		return "";
	QString edgesList;
	QString line = lineTemplate;
	QString const templ = "qMakePair(qMakePair(QString(\"%1\"),QString(\"%2\")),qMakePair(%3,QString(\"%4\")))";
	QString directed = "false";
	foreach(PossibleEdge edge, mPossibleEdges) {
		if (edge.second.first)
			directed = "true";
		QString current = templ.arg(edge.first.first).arg(edge.first.second).arg(directed).arg(edge.second.second);
		edgesList += "<< " + current  + " ";
	}
	line.replace(possibleEdgesListTag, edgesList).replace(elementNameTag, name());
	return line;
}

