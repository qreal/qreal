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

#include "display.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase::robotModel;

Display::Display(const DeviceInfo &info, const PortInfo &port)
	: robotModel::parts::NxtDisplay(info, port)
{
}

void Display::drawPixel(int x, int y)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	Q_UNUSED(x1)
	Q_UNUSED(y1)
	Q_UNUSED(x2)
	Q_UNUSED(y2)
}

void Display::drawRect(int x, int y, int width, int height)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
}

void Display::drawCircle(int x, int y, int radius)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(radius)
}

void Display::printText(int x, int y, const QString &text)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(text)
}

void Display::clearScreen()
{
}

void Display::redraw()
{
}
