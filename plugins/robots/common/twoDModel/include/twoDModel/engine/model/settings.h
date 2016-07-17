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

#pragma once

#include <QtCore/QObject>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {
namespace model {

/// Incapsulates settings used by 2D model.
class TWO_D_MODEL_EXPORT Settings : public QObject
{
	Q_OBJECT

public:
	Settings();

	/// Returns true is user selected realistic physical engine.
	bool realisticPhysics() const;

	/// Returns true is user wants to add some noise to sensors values.
	bool realisticSensors() const;

	/// Returns true is user wants to add some noise to motors work.
	bool realisticMotors() const;

	/// Rereads all settings related to realistic emulation.
	void rereadNoiseSettings();

signals:
	/// Emitted each time when user modifies physical preferences.
	void physicsChanged(bool isRealistic);

private:
	bool mRealisticPhysics = false;
	bool mRealisticSensors = false;
	bool mRealisticMotors = false;
};

}
}
