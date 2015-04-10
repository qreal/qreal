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

#include <qrutils/graphicsUtils/painterInterface.h>
#include <qrutils/graphicsUtils/paintWidget.h>

#include "twoDModel/engine/twoDModelDisplayWidget.h"

namespace Ui {
class TrikDisplayWidget;
}

namespace trik {

class TrikDisplayWidget : public twoDModel::engine::TwoDModelDisplayWidget
{
	Q_OBJECT

public:
	explicit TrikDisplayWidget(QWidget *parent = nullptr);
	~TrikDisplayWidget();

	void setPainter(graphicsUtils::PainterInterface *painter) override;

	void repaintDisplay() override;
	int displayWidth() const override;
	int displayHeight() const override;

	bool buttonIsDown(const QString &buttonId) const override;
	/// sets LED widget's color on 2D model display.
	void setLedColor(const QColor &color);

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	Ui::TrikDisplayWidget *mUi;
	QImage mBackground;
};

}
