/* Copyright 2007-2016 QReal Research Group
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

#include "statPoint.h"
#include "statLine.h"

namespace qReal {
namespace gui {
namespace editor {

/// A factory creating internal ports implemetations from their description in metamodel..
/// @todo: This object in its current sight is simply unneeded. But something tells me that it will be useful
/// when we add circle ports and so on. Is that true?
class PortFactory
{
public:
	/// Produces objects used for internal operations with the point ports described by \a info.
	/// Transfers ownership on created objects.
	QList<StatPoint *> createPorts(const QList<PointPortInfo> &infos) const;

	/// Produces objects used for internal operations with the line ports described by \a info.
	/// Transfers ownership on created objects.
	QList<StatLine *> createPorts(const QList<LinePortInfo> &infos) const;
};

}
}
}
