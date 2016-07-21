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

#include "nullTwoDModelDisplayWidget.h"

using namespace twoDModel::engine;

NullTwoDModelDisplayWidget::NullTwoDModelDisplayWidget(QWidget *parent)
	: twoDModel::engine::TwoDModelDisplayWidget(parent)
{
}

NullTwoDModelDisplayWidget::~NullTwoDModelDisplayWidget()
{
}

void NullTwoDModelDisplayWidget::setPainter(qReal::ui::PainterInterface *painter)
{
	TwoDModelDisplayWidget::setPainter(painter);
}

bool NullTwoDModelDisplayWidget::buttonIsDown(const QString &buttonId) const
{
	Q_UNUSED(buttonId)
	return false;
}

void NullTwoDModelDisplayWidget::repaintDisplay()
{
}

int NullTwoDModelDisplayWidget::displayWidth() const
{
	return 0;
}

int NullTwoDModelDisplayWidget::displayHeight() const
{
	return 0;
}
