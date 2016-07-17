/* Copyright 2016 CyberTech Labs Ltd.
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

#include "changePropertyCommand.h"

#include <qrutils/graphicsUtils/abstractScene.h>

using namespace twoDModel::commands;

ChangePropertyCommand::ChangePropertyCommand(const graphicsUtils::AbstractScene &scene
		, const model::Model &model
		, const QStringList &ids
		, const QString &property
		, const QVariant &value)
	: mScene(scene)
	, mModel(model)
	, mIds(ids)
	, mPropertyName(property)
{
	for (const QString &id : ids) {
		mNewValues[id] = value;
	}
}

bool ChangePropertyCommand::execute()
{
	for (const QString &id : mIds) {
		if (graphicsUtils::AbstractItem *item = mScene.findItem(id)) {
			mOldValues[id] = item->property(qUtf8Printable(mPropertyName));
		}
	}

	return setProperties(mNewValues);
}

bool ChangePropertyCommand::restoreState()
{
	return setProperties(mOldValues);
}

bool ChangePropertyCommand::setProperties(const QMap<QString, QVariant> &values)
{
	for (const QString &id : mIds) {
		if (graphicsUtils::AbstractItem *item = mScene.findItem(id)) {
			item->setProperty(qUtf8Printable(mPropertyName), values[id]);
		}
	}

	return true;
}
