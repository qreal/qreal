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

#include "geometricForms.h"
#include "pathCorrector.h"

namespace qReal {
namespace gestures {

class KeyBuilder
{
public:
	static Key getKey(const PathVector &mousePath, int heightSize, int widthSize);

private:
	static void rasterizeSegment(const SquarePos &pos1, const SquarePos &pos2, Key *segment);
};

}
}
