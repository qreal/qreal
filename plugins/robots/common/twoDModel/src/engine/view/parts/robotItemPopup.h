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

#include <qrutils/graphicsUtils/itemPopup.h>

class QAbstractButton;
class QSpinBox;

namespace twoDModel {
namespace view {

class RobotItem;

/// A popup for RobotItem on 2D model scene.
/// Contains spin box for setting robot`s marker thickness and a couple of position management buttons.
class RobotItemPopup : public graphicsUtils::ItemPopup
{
	Q_OBJECT

public:
	explicit RobotItemPopup(graphicsUtils::AbstractScene &scene, QWidget *parent = 0);
	~RobotItemPopup();

signals:
	/// Emitted when user decides to follow or unfollow robot item.
	void followingChanged(bool enabled);

	/// Emitted when user clicked restore robot position button.
	void restoreRobotPositionClicked();

private:
	bool suits(QGraphicsItem *item) override;
	bool attachTo(QGraphicsItem *item) override;
	bool attachTo(const QList<QGraphicsItem *> &items) override;

	void initWidget();
	QWidget *initFollowButton();
	QWidget *initReturnButton();
	QAbstractButton *initButton(const QString &icon, const QString &toolTip);
	QWidget *initSpinBox();

	QAbstractButton *mFollowButton;  // Takes ownership
	QAbstractButton *mReturnButton;  // Takes ownership
	QSpinBox *mSpinBox;  // Takes ownership
	RobotItem *mCurrentItem = nullptr;  // Does not take ownership
};

}
}
