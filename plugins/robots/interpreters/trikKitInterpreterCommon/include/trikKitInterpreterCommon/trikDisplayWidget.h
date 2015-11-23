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

#include <QtGui/QImage>

#include <qrutils/widgets/paintWidget.h>
#include <twoDModel/engine/twoDModelDisplayWidget.h>

#include "declSpec.h"

namespace Ui {
class TrikDisplayWidget;
}

namespace trik {

class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT TrikDisplayWidget : public twoDModel::engine::TwoDModelDisplayWidget
{
	Q_OBJECT

public:
	explicit TrikDisplayWidget(QWidget *parent = nullptr);
	~TrikDisplayWidget();

	void reset() override;

	void setPainter(qReal::ui::PainterInterface *painter) override;

	void repaintDisplay() override;
	int displayWidth() const override;
	int displayHeight() const override;

	bool buttonIsDown(const QString &buttonId) const override;

	/// Returns the current color of led emulator in 2D model.
	QColor ledColor() const;

	/// Sets the color of led emulator in 2D model.
	void setLedColor(const QColor &color);

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	void resetLed();

	Ui::TrikDisplayWidget *mUi;
	QImage mBackground;
};

}
