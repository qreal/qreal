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

#include <QtCore/QList>
#include <QtCore/QPointF>

const qreal sense = 1;
const qreal speedKoef = 0.0275;

namespace qReal {
namespace gestures {

class PathCorrector
{
public:
	static QList<QPoint> correctPath(QList<QPoint> const &path);
	static QList<QPoint> getMousePath(QList<QPoint> const &path);
};

}
}
