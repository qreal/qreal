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

#include "scalableItem.h"

using namespace utils;

ScalableCoordinate ScalableItem::initCoordinate(const QString &coordinate, int maxValue)
{
	ScalableCoordinate field;
	QString coord = coordinate;
	if (coord.endsWith("a")) {
		coord.remove(coord.length() - 1, 1);
		field = ScalableCoordinate(static_cast<qreal>(coord.toInt()) / maxValue, true);
	} else if (coord.endsWith("%")) {
		coord.remove(coord.length() - 1, 1);
		field = ScalableCoordinate(static_cast<qreal>(coord.toInt()) / 100, false);
	} else {
		field = ScalableCoordinate(static_cast<qreal>(coord.toInt()) / maxValue, false);
	}
	return field;
}
