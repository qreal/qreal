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

/** @brief line port description */
class StatLine : public PortInterface
{
public:
	StatLine(const QLineF &line, bool propX1, bool propY1, bool propX2, bool propY2, int initWidth, int initHeight
			, PortImpl *impl);

	operator QLineF () const;
	void operator= (const QLineF &l);

	virtual void paint(QPainter *painter, const QRectF &contents) const;
	QLineF transformForContents(const QRectF &contents) const;

private:
	QLineF mLine;

	bool mPropX1;
	bool mPropY1;
	bool mPropX2;
	bool mPropY2;

	int mInitWidth;
	int mInitHeight;
};

}
}
}
