/* Copyright 2007-2015 QReal Research Group
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

#include "type.h"

#include <QtCore/QDebug>

#include "property.h"

Type::Type(bool isResolved, Diagram *diagram)
	: mResolvingFinished(isResolved), mDiagram(diagram)
{}

Type::~Type()
{
	qDeleteAll(mProperties);
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
