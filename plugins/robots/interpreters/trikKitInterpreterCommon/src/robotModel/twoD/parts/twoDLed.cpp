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

#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDLed.h"

#include "trikKitInterpreterCommon/trikDisplayWidget.h"

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;

TwoDLed::TwoDLed(const DeviceInfo &info
		, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikLed(info, port)
	, mEngine(engine)
{
	connect(this, &TwoDLed::colorChanged, this, [=](const QColor &color) { emit propertyChanged("color", color); });
}

QColor TwoDLed::color() const
{
	const auto display = dynamic_cast<TrikDisplayWidget *>(mEngine.display());
	Q_ASSERT(display);
	return display->ledColor();
}

void TwoDLed::setColor(const QColor &color)
{
	auto display = dynamic_cast<TrikDisplayWidget *>(mEngine.display());
	Q_ASSERT(display);
	display->setLedColor(color);
	emit colorChanged(color);
}

void TwoDLed::setColor(const QString &color)
{
	const QColor newColor = color == "off" ? QColor(Qt::gray) : QColor(color);
	setColor(newColor);
}
