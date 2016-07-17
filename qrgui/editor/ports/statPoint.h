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

#include <metaMetaModel/portHelpers.h>

class QRectF;
class QPainter;

namespace qReal {
namespace gui {
namespace editor {

/** @brief point port description */
class StatPoint
{
public:
	explicit StatPoint(const PointPortInfo &info);

	operator QPointF() const;
	void operator= (const QPointF &p);

	void paint(QPainter *painter, const QRectF &contents) const;
	QPointF transformForContents(const QRectF &contents) const;

	/// Returns type of this port to filter out links that can be connected to it.
	QString type() const;

private:
	QPointF mPoint;

	bool mPropX;
	bool mPropY;

	int mInitWidth;
	int mInitHeight;

	QString mType;
};

}
}
}
