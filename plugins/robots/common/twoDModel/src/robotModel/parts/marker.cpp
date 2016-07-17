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

#include "twoDModel/robotModel/parts/marker.h"

#include <QtGui/QColor>

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;

Marker::Marker(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: Device(info, port)
	, mEngine(engine)
{
	connect(this, &Marker::isDownChanged, this, [=](bool isDown) { emit propertyChanged("isDown", isDown); });
	connect(this, &Marker::colorChanged, this, [=](const QColor &color) { emit propertyChanged("color", color); });
}

void Marker::down(const QColor &color)
{
	mEngine.markerDown(color);
	emit isDownChanged(true);
	emit colorChanged(color);
}

void Marker::up()
{
	mEngine.markerUp();
	emit isDownChanged(false);
}

bool Marker::isDown() const
{
	return mEngine.isMarkerDown();
}

void Marker::setDown(bool isDown)
{
	if (isDown) {
		down(Qt::black);
	} else {
		up();
	}
}
