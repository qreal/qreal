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

#include "generatorBase/converters/enumConverterBase.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as repository property of the 'BreakMode'
/// type. Mapped values must be specified in "engines/breakMode/break.t" and
/// "engines/breakMode/float.t" templates.
class BreakModeConverter : public EnumConverterBase
{
public:
	explicit BreakModeConverter(const QStringList &pathsToTemplates);
};

}
}
