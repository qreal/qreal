/* Copyright 2015-2016 Dmitry Mordvinov
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

#include <QtGui/QPolygon>

#include "elementInfo.h"

namespace qReal {

/// Container for convenient storing edge element data for passing it into methods with just one parameter.
class EdgeInfo : public ElementInfo
{
public:
	EdgeInfo();

	Id srcId;
	Id dstId;

	qreal portFrom;
	qreal portTo;

	QPolygon configuration;
	int shapeType;

	QDataStream &serialize(QDataStream &out) const override;
	QDataStream &deserialize(QDataStream &in) override;

	bool equals(const ElementInfo &other) const override;

	/// Places fields into graphical and logical properties map, returns new ElementInfo instance.
	ElementInfo convertToSimpleInfo() const;
};

}

QDataStream &operator<< (QDataStream &out, const qReal::EdgeInfo &data);
QDataStream &operator>> (QDataStream &in, qReal::EdgeInfo &data);
bool operator== (const qReal::EdgeInfo &first, const qReal::EdgeInfo &second);
