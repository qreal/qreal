/* Copyright 2015 CyberTech Labs Ltd.
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

#include "twoDLed.h"

#include "src/ev3DisplayWidget.h"

using namespace ev3::robotModel::twoD::parts;
using namespace kitBase::robotModel;

TwoDLed::TwoDLed(const DeviceInfo &info
		, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::Ev3Led(info, port)
	, mEngine(engine)
{
	connect(this, &TwoDLed::colorChanged, this, [=](const QColor &color) { emit propertyChanged("color", color); });
}

QColor TwoDLed::color() const
{
	/// @todo
	return QColor();
}

void TwoDLed::setColor(const QColor &color)
{
	/// @todo
	Q_UNUSED(color)
}

void TwoDLed::setColor(ev3::robotModel::parts::Ev3LedColor color)
{
	/// @todo
	Q_UNUSED(color)
}
