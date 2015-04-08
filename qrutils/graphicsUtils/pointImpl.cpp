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

#include "pointImpl.h"

using namespace graphicsUtils;

PointImpl::PointImpl()
{
}

QRectF PointImpl::boundingRect(qreal x, qreal y, qreal radius, const int scalingDrift) const
{
	return QRectF(x - radius, y - radius, 2 * radius, 2 * radius).adjusted(-scalingDrift, -scalingDrift, scalingDrift
			, scalingDrift);
}

void PointImpl::drawItem(QPainter* painter, qreal x, qreal y, qreal radius)
{
	painter->drawEllipse(QRectF(x - radius, y - radius, 2 * radius, 2 * radius));
}

void PointImpl::drawFieldForResizeItem(QPainter* painter)
{
	Q_UNUSED(painter);
}
