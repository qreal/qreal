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

#include "trikKitInterpreterCommon/trikDisplayWidget.h"
#include "ui_trikDisplayWidget.h"

#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>

using namespace trik;

TrikDisplayWidget::TrikDisplayWidget(QWidget *parent)
	: twoDModel::engine::TwoDModelDisplayWidget(parent)
	, mUi(new Ui::TrikDisplayWidget)
	, mBackground(":/icons/trik_controller.png", "PNG")
{
	mUi->setupUi(this);
	resetLed();
}

TrikDisplayWidget::~TrikDisplayWidget()
{
	delete mUi;
}

void TrikDisplayWidget::setPainter(qReal::ui::PainterInterface *painter)
{
	TwoDModelDisplayWidget::setPainter(painter);
	mUi->display->setPainter(painter);
}

void TrikDisplayWidget::repaintDisplay()
{
	mUi->display->repaint();
}

int TrikDisplayWidget::displayWidth() const
{
	return mUi->display->width();
}

int TrikDisplayWidget::displayHeight() const
{
	return mUi->display->height();
}

bool TrikDisplayWidget::buttonIsDown(const QString &buttonPort) const
{
	if (buttonPort == "Left") {
		return mUi->buttonLeft->isDown();
	} else if (buttonPort == "Right") {
		return mUi->buttonRight->isDown();
	} else if (buttonPort == "Up") {
		return mUi->buttonUp->isDown();
	} else if (buttonPort == "Down") {
		return mUi->buttonDown->isDown();
	} else if (buttonPort == "Enter") {
		return mUi->buttonEnter->isDown();
	} else if (buttonPort == "Esc") {
		return mUi->buttonEscape->isDown();
	} else if (buttonPort == "Power") {
		return mUi->buttonPower->isDown();
	}

	QLOG_WARN() << "Button on port" << buttonPort << "is not supported by 2d model";

	return false;
}

QColor TrikDisplayWidget::ledColor() const
{
	return mUi->led->palette().color(QPalette::Background);
}

void TrikDisplayWidget::setLedColor(const QColor &color)
{
	QPalette backgroundPalette(palette());
	backgroundPalette.setColor(QPalette::Background, color);
	backgroundPalette.setColor(QPalette::Base, color);
	mUi->led->setPalette(backgroundPalette);
	mUi->led->show();
}

void TrikDisplayWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, width(), height()), mBackground);
}

void TrikDisplayWidget::reset()
{
	TwoDModelDisplayWidget::reset();
	resetLed();
}

void TrikDisplayWidget::resetLed()
{
	setLedColor("red");
}
