#include "type.h"
#include "property.h"

#include <QDebug>

Type::Type(bool isResolved, Diagram *diagram)
	: mResolvingFinished(isResolved), mDiagram(diagram)
{}

Type::~Type()
{
	foreach (Property *property, mProperties.values())
		if (property)
			delete property;
}

bool Type::init(QDomElement const &element, QString const &context)
{
	mName = element.attribute("name");
	mContext = context;
	mNativeContext = context;
	if (mName == "") {
		qDebug() << "ERROR: anonymous type found";
		return false;
	}
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

QString Type::qualifiedName() const
{
	return mContext + "::" + mName;
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
	foreach (QString propertyName, mProperties.keys())
		type->mProperties.insert(propertyName, mProperties[propertyName]->clone());
	type->mResolvingFinished = mResolvingFinished;
	type->mDiagram = mDiagram;
}
