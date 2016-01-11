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

ReshapeCommand::ReshapeCommand(graphicsUtils::AbstractScene &scene, const model::Model &model, const QString &id)
	: mScene(scene)
	, mModel(model)
	, mId(id)
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

bool ReshapeCommand::findItem()
{
	return (mItem = mScene.findItem(mId)) != nullptr;
}

QDomElement ReshapeCommand::snapshotElement()
{
	if (!findItem()) {
		return QDomElement();
	}

	QDomElement temporalParent = mXmlFactory.createElement("temporalParent");
	return mItem->serialize(temporalParent);
}

void ReshapeCommand::setConfiguration(const QDomElement &configuration)
{
	if (findItem()) {
		mItem->deserialize(configuration);
		mScene.update();
	}
}

void ReshapeCommand::startTracking()
{
	TrackingEntity::startTracking();
	mOldConfiguration = snapshotElement();
}

void ReshapeCommand::stopTracking()
{
	TrackingEntity::stopTracking();
	mNewConfiguration = snapshotElement();
}

bool ReshapeCommand::modificationsHappened() const
{
	return mOldConfiguration != mNewConfiguration;
}
