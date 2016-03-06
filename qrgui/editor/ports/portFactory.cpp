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

#include "portFactory.h"

#include "editor/ports/statLine.h"
#include "editor/ports/statPoint.h"

using namespace qReal::gui::editor;

PortInterface * PortFactory::createPort(const QLineF &line, bool propX1, bool propY1, bool propX2, bool propY2
		, int initWidth, int initHeight, PortImpl *impl) const
{
	return new StatLine(line, propX1, propY1, propX2, propY2, initWidth, initHeight, impl);
}

PortInterface * PortFactory::createPort(const QPointF &point, bool propX, bool propY
		, int initWidth, int initHeight, PortImpl *impl) const
{
	return new StatPoint(point, propX, propY, initWidth, initHeight, impl);
}
