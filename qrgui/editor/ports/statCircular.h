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

/** @brief circular port description */
class StatCircular : public PortInterface
{
public:
	StatCircular(const QPointF &point, const qreal &r,bool propX, bool propY, int initWidth
			, int initHeight, PortImpl *impl);

	virtual void paint(QPainter *painter, const QRectF &contents) const;
	QLineF transformForContents(const QRectF &contents) const;

private:
	QPointF mCircular;

	bool mPropX;
	bool mPropY;
	const qreal mR;
	int mInitWidth;
	int mInitHeight;
};

}
}
}
