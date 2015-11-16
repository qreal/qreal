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

#include "editor/private/edgeArrangeCriteria.h"

using namespace qReal::gui::editor;

EdgeArrangeCriteria::EdgeArrangeCriteria(int edgeType, qreal yOffset, qreal xOffset)
		: mEdgeType(edgeType)
		, mYOffset(yOffset)
		, mXOffset(xOffset)
{
}

bool EdgeArrangeCriteria::operator<(const EdgeArrangeCriteria &other) const
{
	QPair<int, QPair<qreal, qreal> > first = qMakePair(mEdgeType, qMakePair(mYOffset, mXOffset));
	QPair<int, QPair<qreal, qreal> > second = qMakePair(other.mEdgeType, qMakePair(other.mYOffset, other.mXOffset));

	return first < second;
}
