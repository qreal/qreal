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

#include <QtCore/QMetaType>

namespace kitBase {
namespace robotModel {

/// Direction of a port and device (input or output). Note that in some kits (like NXT) two devices can be
/// connected to one port simultaneously, like motor and encoder. Direction is used to differentiate them.
enum Direction
{
	/// Device provides data (like sensors or encoders).
	input = 0
	/// Device accepts data (like motors or lamps).
	, output
};

}
}

Q_DECLARE_METATYPE(kitBase::robotModel::Direction)
