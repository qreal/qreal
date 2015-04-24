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

#include <QtCore/QLineF>

#include "gridDrawer.h"

using namespace graphicsUtils;

GridDrawer::GridDrawer()
{
}

void GridDrawer::drawGrid(QPainter *painter, const QRectF &rect, const int indexGrid)
{
	const int left = static_cast<int>(rect.left());
	const int right = static_cast<int>(rect.right());
	const int top = static_cast<int>(rect.top());
	const int bottom = static_cast<int>(rect.bottom());

	const int startX = left / indexGrid * indexGrid;
	const int startY = top / indexGrid * indexGrid;

	for (int i = startX; i <= right; i += indexGrid) {
		painter->drawLine(i, top, i, bottom);
	}

	for (int i = startY; i <= bottom; i += indexGrid) {
		painter->drawLine(left, i, right, i);
	}
}
