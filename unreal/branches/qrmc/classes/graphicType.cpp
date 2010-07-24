#include "graphicType.h"
#include "property.h"
#include "diagram.h"
#include "utils/nameNormalizer.h"
#include "../utils/outFile.h"

#include <QDebug>

using namespace utils;
using namespace qReal;

GraphicType::ContainerProperties::ContainerProperties() : isSortContainer(false), sizeOfForestalling(0),
	sizeOfChildrenForestalling(0), isChildrenMovable(true), isMinimizingToChildren(false), isClass(false), isMaximizingChildren(false)
{}

GraphicType::ResolvingHelper::ResolvingHelper(bool &resolvingFlag)
	: mResolvingFlag(resolvingFlag)
{
	mResolvingFlag = true;
}

GraphicType::GraphicType(Diagram *diagram, qrRepo::RepoApi *api, const qReal::Id &id)
	: Type(false, diagram, api, id), mWidth(-1), mHeight(-1), mResolving(false)
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

	foreach(Id id, mApi->children(mId)) {
		if (id.element() == "MetaEntityParent") {
			QString parentName = mApi->name(id);
			if (!mParents.contains(parentName))
				mParents.append(parentName);
			else {
				qDebug() << "ERROR: parent of node" << qualifiedName() << "duplicated";
				return false;
			}
		} else if (id.element() == "MetaEntity_Attribute") {
			Property *property = new Property(mApi, id);
			if (!property->init()) {
				delete property;
				continue;
			}
			if (!addProperty(property))
				return false;
		}
	}
	return true;
}

GraphicType::ResolvingHelper::~ResolvingHelper()
{
	mResolvingFlag = false;
}

void GraphicType::copyFields(GraphicType *type) const
{
	Type::copyFields(type);
//	type->mElement = mElement;
//	type->mGraphics = mGraphics;
	type->mHeight = mHeight;
//r	type->mLogic = mLogic;
	type->mParents = mParents;
	type->mIsVisible = mIsVisible;
	type->mWidth = mWidth;
	type->mContainerProperties = mContainerProperties;
	type->mContains = mContains;
}

bool GraphicType::resolve()
{
	if (mResolvingFinished)
		return true;

	ResolvingHelper helper(mResolving);
	mParents.removeDuplicates();
	foreach (QString parentName, mParents) {
		// searching for parents in native context. if it was imported, references will remain valid
		QString qualifiedParentName = nativeContext() + "::" + parentName;

		Type *parent = mDiagram->findType(qualifiedParentName);
		if (parent == NULL) {
			// didn't find in local context, try global
			parent = mDiagram->findType(qualifiedParentName);
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

//		GraphicType* gParent = dynamic_cast<GraphicType*>(parent);
//		if (gParent)
//			foreach (PossibleEdge pEdge,gParent->mPossibleEdges) {
//				mPossibleEdges.append(qMakePair(pEdge.first,qMakePair(pEdge.second.first,name())));
//			}
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
