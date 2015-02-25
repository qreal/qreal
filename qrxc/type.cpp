#include "type.h"

#include <QtCore/QDebug>

#include "property.h"

Type::Type(bool isResolved, Diagram *diagram)
	: mResolvingFinished(isResolved), mDiagram(diagram)
{}

Type::~Type()
{
	foreach (Property *property, mProperties.values())
		if (property)
			delete property;
}

bool Type::init(const QDomElement &element, const QString &context)
{
	mName = element.attribute("name");
	mContext = context;
	mNativeContext = context;
	if (mName == "") {
		qDebug() << "ERROR: anonymous type found";
		return false;
	}
	qDebug() << "mName:" << element.attribute("name");
	mDisplayedName = element.attribute("displayedName", mName);
	mPath = element.attribute("path", "");
	return true;
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
	return mContext + "::" + mName;
}

QString Type::nativeContext() const
{
	return mNativeContext;
}

void Type::setContext(const QString &newContext)
{
	mContext = newContext;
}

void Type::setName(const QString &name)
{
	mName = name;
}

QString Type::displayedName() const
{
	return mDisplayedName;
}

void Type::setDisplayedName(const QString &displayedName)
{
	mDisplayedName = displayedName;
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
