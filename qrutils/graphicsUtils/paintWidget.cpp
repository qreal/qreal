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

#include "paintWidget.h"
#include "painterInterface.h"

using namespace graphicsUtils;

PaintWidget::PaintWidget(QWidget *parent)
	: QWidget(parent)
{
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);

	for (PainterInterface * const painterIface : mPainters) {
		painterIface->paint(&painter);
	}
}

void PaintWidget::appendPainter(PainterInterface *painter)
{
	mPainters << painter;
}
