/* Copyright 2017 CyberTech Labs Ltd.
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

#include <QtGui/QColor>

#include "trikKitInterpreterCommon/trikEmulation/trikProxyMarker.h"

using namespace trik;

TrikProxyMarker::TrikProxyMarker(twoDModel::robotModel::parts::Marker *marker)
	: mMarker(marker)
{
}

void TrikProxyMarker::down(const QString &color)
{
	mMarker->down(color == "darkYellow" ? QColor(Qt::darkYellow) : QColor(color));
}

void TrikProxyMarker::up()
{
	mMarker->up();
}

bool TrikProxyMarker::isDown() const
{
	return mMarker->isDown();
}

void TrikProxyMarker::setDown(bool isDown)
{
	mMarker->setDown(isDown);
}
