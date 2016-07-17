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

#include "nxtDisplayWidget.h"

#include "ui_nxtDisplayWidget.h"

#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>

using namespace nxt;

NxtDisplayWidget::NxtDisplayWidget(QWidget *parent)
	: twoDModel::engine::TwoDModelDisplayWidget(parent)
	, mUi(new Ui::NxtDisplayWidget)
	, mBackground(":/icons/background.png", "PNG")
{
	mUi->setupUi(this);
}

NxtDisplayWidget::~NxtDisplayWidget()
{
	delete mUi;
}

bool NxtDisplayWidget::buttonIsDown(const QString &buttonPort) const
{
	if (buttonPort == "Left") {
		return mUi->leftButton->isDown();
	} else if (buttonPort == "Right") {
		return mUi->rightButton->isDown();
	} else if (buttonPort == "Enter") {
		return mUi->centralButton->isDown();
	} else if (buttonPort == "Escape") {
		return mUi->bottomButton->isDown();
	}

	QLOG_WARN() << "Button on port" << buttonPort << "is not supported by 2d model";

	return false;
}

void NxtDisplayWidget::setPainter(qReal::ui::PainterInterface *painter)
{
	TwoDModelDisplayWidget::setPainter(painter);
	mUi->display->setPainter(painter);
}

void NxtDisplayWidget::repaintDisplay()
{
	mUi->display->repaint();
}

int NxtDisplayWidget::displayWidth() const
{
	return mUi->display->width();
}

int NxtDisplayWidget::displayHeight() const
{
	return mUi->display->height();
}

void NxtDisplayWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, width(), height()), mBackground);
}
