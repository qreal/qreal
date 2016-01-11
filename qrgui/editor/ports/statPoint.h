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

#include "plugins/editorPluginInterface/portHelpers.h"

namespace qReal {
namespace gui {
namespace editor {

/** @brief point port description */
class StatPoint : public PortInterface
{
public:
	StatPoint(const QPointF &point, bool propX, bool propY, int initWidth, int initHeight, PortImpl *impl);

	operator QPointF() const;
	void operator= (const QPointF &p);

	virtual void paint(QPainter *painter, const QRectF &contents) const;
	QPointF transformForContents(const QRectF &contents) const;

private:
	QPointF mPoint;

	bool mPropX;
	bool mPropY;

	int mInitWidth;
	int mInitHeight;
};

}
}
}
