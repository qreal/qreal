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

#include "reshapeCommand.h"

#include <qrutils/graphicsUtils/abstractScene.h>
#include <qrutils/graphicsUtils/rotateItem.h>

#include "twoDModel/engine/model/model.h"

using namespace twoDModel::commands;

ReshapeCommand::ReshapeCommand(graphicsUtils::AbstractScene &scene, const model::Model &model, const QStringList &ids)
	: mScene(scene)
	, mModel(model)
	, mIds(ids)
{
}

bool ReshapeCommand::execute()
{
	if (!mTrackStopped) {
		return true;
	}

	setConfiguration(mNewConfiguration);
	return true;
}

bool ReshapeCommand::restoreState()
{
	if (!mTrackStopped) {
		return true;
	}

	setConfiguration(mOldConfiguration);
	return true;
}

void ReshapeCommand::takeSnapshot(QMap<QString, QDomElement> &target)
{
	target.clear();
	for (const QString &id : mIds) {
		if (const graphicsUtils::AbstractItem *item = mScene.findItem(id)) {
			QDomElement temporalParent = mXmlFactory.createElement("temporalParent");
			target[id] = item->serialize(temporalParent);
		}
	}
}

void ReshapeCommand::setConfiguration(const QMap<QString, QDomElement> &configuration)
{
	for (const QString &id : mIds) {
		if (graphicsUtils::AbstractItem * const item = mScene.findItem(id)) {
			item->deserialize(configuration[id]);
		}
	}

	mScene.update();
}

void ReshapeCommand::startTracking()
{
	TrackingEntity::startTracking();
	takeSnapshot(mOldConfiguration);
}

void ReshapeCommand::stopTracking()
{
	TrackingEntity::stopTracking();
	takeSnapshot(mNewConfiguration);
}

bool ReshapeCommand::modificationsHappened() const
{
	return mOldConfiguration != mNewConfiguration;
}
