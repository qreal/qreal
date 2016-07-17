/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include "property.h"
#include "qrmc/utils/nameNormalizer.h"

#include <QtCore/QDebug>

using namespace qrmc;

Type::Type(bool isResolved, const Diagram &diagram, const qrRepo::LogicalRepoApi &api, const qReal::Id &id)
	: mResolvingFinished(isResolved)
	, mDiagram(&diagram)
	, mId(id)
	, mApi(api)
{
}

Type::~Type()
{
	qDeleteAll(mProperties);
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

const Diagram &Type::diagram() const
{
	return *mDiagram;
}

const QMap<QString, Property *> &Type::properties() const
{
	return mProperties;
}

void Type::setDiagram(Diagram &diagram)
{
	mDiagram = &diagram;
}

void Type::copyFields(Type *type) const
{
	type->mName = mName;
	type->mContext = mContext;
	type->mNativeContext = mNativeContext;
	type->mDisplayedName = mDisplayedName;
	for (const QString &propertyName : mProperties.keys()) {
		type->mProperties.insert(propertyName, mProperties[propertyName]->clone());
	}

	type->mResolvingFinished = mResolvingFinished;
	type->mDiagram = mDiagram;
}

bool Type::init(const QString &context)
{
	mName = mApi.name(mId);
	mDisplayedName = mApi.stringProperty(mId, "displayedName");
	if (mDisplayedName.isEmpty()) {
		mDisplayedName = mName;
	}

	mName = NameNormalizer::normalize(mName);
	mContext = context;
	mNativeContext = context;
	if (mName.isEmpty()) {
		qDebug() << "ERROR: anonymous type found";
		return false;
	}

	if (mApi.hasProperty(mId, "path")) {
		mPath = mApi.stringProperty(mId, "path");
	}

	return true;
}

QString Type::generateNames(const QString &lineTemplate) const
{
	if (displayedName().isEmpty() || !isGraphicalType()) {
		return "";
	}

	QString result = lineTemplate;
	result.replace(elementNameTag
			, name()).replace(elementDisplayedNameTag
			, displayedName()).replace(diagramNameTag
			, mContext);

	return result;
}

QString Type::generateMouseGestures(const QString &lineTemplate) const
{
	if (mPath.isEmpty()) {
		return "";
	}

	QString result = lineTemplate;
	result.replace(elementNameTag, name()).replace(gesturePathTag, mPath).replace(diagramNameTag, mContext);
	return result;
}

QString Type::generateFactory(const QString &lineTemplate) const
{
	if (!isGraphicalType()) {
		return "";
	}

	QString result = lineTemplate;
	result.replace(elementNameTag, mName);
	return result;
}
