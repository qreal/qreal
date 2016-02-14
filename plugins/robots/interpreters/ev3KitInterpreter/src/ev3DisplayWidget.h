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

#include <QtWidgets/QWidget>

#include <qrutils/widgets/paintWidget.h>

#include "twoDModel/engine/twoDModelDisplayWidget.h"

namespace Ui
{
class Ev3DisplayWidget;
}

namespace ev3 {

class Ev3DisplayWidget : public twoDModel::engine::TwoDModelDisplayWidget
{
	Q_OBJECT

public:
	explicit Ev3DisplayWidget(QWidget *parent = 0);
	~Ev3DisplayWidget();

	void setPainter(qReal::ui::PainterInterface *painter) override;

	bool buttonIsDown(const QString &buttonId) const override;

	void repaintDisplay();
	int displayWidth() const;
	int displayHeight() const;

protected:
	void paintEvent(QPaintEvent *);

private:
	Ui::Ev3DisplayWidget *mUi;
	QImage mBackground;
};

}
