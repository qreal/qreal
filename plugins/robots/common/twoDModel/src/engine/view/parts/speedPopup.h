/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

class QLabel;
class QAbstractButton;
class QTimer;

namespace twoDModel {
namespace view {

/// A small popup bringing feedback to user when he changes 2D model interpretation speed.
class SpeedPopup : public QWidget
{
	Q_OBJECT

public:
	explicit SpeedPopup(QWidget *parent);

public slots:

	void setSpeed(int speed);

signals:
	/// Emitted when user wants to reset speed factor to default value.
	void resetToDefault();

private:
	void updateDueToLayout();

	QLabel *mText;
	QAbstractButton *mResetButton;
	QTimer *mHideTimer;
};

}
}
