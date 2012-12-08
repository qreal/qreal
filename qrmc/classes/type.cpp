#include "type.h"
#include "property.h"
#include "../utils/nameNormalizer.h"

#include <QDebug>

using namespace qrmc;

Type::Type(bool isResolved, Diagram *diagram, qrRepo::RepoApi *api, const qReal::Id &id)
	: mResolvingFinished(isResolved), mDiagram(diagram), mId(id), mApi(api)
{
}

Type::~Type()
{
	foreach (Property *property, mProperties.values())
		if (property)
			delete property;
}

bool Type::isResolved() const
{
	return mResolvingFinished;
}

bool Type::isResolving() const
{
	return false;
}

QString Type::name() const
{
	return mName;
}

QString Type::path() const
{
	return mPath;
}

QString Type::qualifiedName() const
{
	return NameNormalizer::normalize(mContext) + "::" + mName;
}

QString Type::nativeContext() const
{
	return mNativeContext;
}

void Type::setContext(QString const &newContext)
{
	mContext = newContext;
}

void Type::setName(QString const &name)
{
	mName = name;
}

QString Type::displayedName() const
{
	return mDisplayedName;
}

void Type::setDisplayedName(QString const &displayedName)
{
	mDisplayedName = displayedName;
}

Diagram *Type::diagram() const
{
	return mDiagram;
}

QMap<QString, Property*> Type::properties() const
{
	return mProperties;
}

void Type::setDiagram(Diagram *diagram)
{
	mDiagram = diagram;
}

void Type::copyFields(Type *type) const
{
	type->mName = mName;
	type->mContext = mContext;
	type->mNativeContext = mNativeContext;
	type->mDisplayedName = mDisplayedName;
	foreach (QString propertyName, mProperties.keys())
		type->mProperties.insert(propertyName, mProperties[propertyName]->clone());
	type->mResolvingFinished = mResolvingFinished;
	type->mDiagram = mDiagram;
}

bool Type::init(QString const &context)
{
	mName = mApi->name(mId);
	mDisplayedName = mApi->stringProperty(mId, "displayedName");
	if (mDisplayedName.isEmpty())
		mDisplayedName = mName;
	mName = NameNormalizer::normalize(mName);
	mContext = context;
	mNativeContext = context;
	if (mName == "") {
		qDebug() << "ERROR: anonymous type found";
		return false;
	}
	if (mApi->hasProperty(mId, "path"))
		mPath = mApi->stringProperty(mId, "path");
	return true;
}

QString Type::generateNames(QString const &lineTemplate) const
{
	if (displayedName().isEmpty() || !isGraphicalType())
		return "";
	QString result = lineTemplate;
	result.replace(elementNameTag, name()).replace(elementDisplayedNameTag, displayedName()).replace(diagramNameTag, mContext);
	return result;
}

QString Type::generateMouseGestures(QString const &lineTemplate) const
{
	if (mPath.isEmpty())
		return "";
	QString result = lineTemplate;
	result.replace(elementNameTag, name()).replace(gesturePathTag, mPath).replace(diagramNameTag, mContext);
	return result;
}

QString Type::generateFactory(const QString &lineTemplate) const
{
	if (!isGraphicalType())
		return "";
	QString result = lineTemplate;
	result.replace(elementNameTag, mName);
	return result;
}
