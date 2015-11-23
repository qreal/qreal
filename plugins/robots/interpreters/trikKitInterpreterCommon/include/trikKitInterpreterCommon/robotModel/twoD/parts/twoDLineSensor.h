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

#include <QtGui/QRgb>

#include <twoDModel/engine/twoDModelEngineInterface.h>
#include <trikKit/robotModel/parts/trikLineSensor.h>

#include "trikKitInterpreterCommon/declSpec.h"

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

/// Stub for TRIK color sensor for interpreter.
/// Does nothing until line sensor emulation will be requested for 2D model.
class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT LineSensor : public robotModel::parts::TrikLineSensor
{
	Q_OBJECT

public:
	LineSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port, twoDModel::engine::TwoDModelEngineInterface &engine);

	void init() override;
	void detectLine() override;
	void read() override;

private:
	bool closeEnough(QRgb color) const;

	twoDModel::engine::TwoDModelEngineInterface &mEngine;
	QRgb mLineColor;
};

}
}
}
}
