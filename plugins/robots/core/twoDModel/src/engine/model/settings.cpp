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

#include "twoDModel/engine/model/settings.h"

#include <qrkernel/settingsManager.h>

using namespace twoDModel::model;

Settings::Settings()
{
	rereadNoiseSettings();
}

bool Settings::realisticPhysics() const
{
	return mRealisticPhysics;
}

bool Settings::realisticSensors() const
{
	return mRealisticSensors;
}

bool Settings::realisticMotors() const
{
	return mRealisticMotors;
}

void Settings::rereadNoiseSettings()
{
	const bool oldPhysics = mRealisticPhysics;
	mRealisticPhysics = qReal::SettingsManager::value("2DModelRealisticPhysics").toBool();
	if (oldPhysics != mRealisticPhysics) {
		emit physicsChanged(mRealisticPhysics);
	}

	mRealisticSensors = qReal::SettingsManager::value("enableNoiseOfSensors").toBool();
	mRealisticMotors = qReal::SettingsManager::value("enableNoiseOfMotors").toBool();
}
