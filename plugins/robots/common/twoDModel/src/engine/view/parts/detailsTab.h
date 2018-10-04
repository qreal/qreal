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

#include <QtWidgets/QTreeWidget>

namespace twoDModel {
namespace view {

/// A widget for beautiful placement of different setting and robot parts in 2D model right panel.
class DetailsTab : public QTreeWidget
{
	Q_OBJECT

public:
	explicit DetailsTab(QWidget *parent = 0);
	~DetailsTab() override;

public slots:
	/// Shows display item in corresponding section. If nullptr is passed the whole section will be hidden.
	void setDisplay(QWidget *widget);

	/// Shows devices configurer widget in corresponding section.
	void setDevicesConfigurer(QWidget *widget);

	/// Shows motors configurer widget in corresponding section.
	void setMotorsConfigurer(QWidget *widget);

	/// Shows physics settings widget in corresponding section.
	void setPhysicsSettings(QWidget *widget);

	/// Shows 2d model parameters widget in corresponding section.
	void setParamsSettings(QWidget *widget);

	/// Shows or hides all sections related to robot`s devices configuration (not motors).
	void setDevicesSectionsVisible(bool visible);

	/// Shows or hides all sections related to robot`s motors configuration.
	void setMotorsSectionsVisible(bool visible);

	/// Shows or hides all sections related to model physics configuration.
	void setPhysicsSectionsVisible(bool visible);

private:
	void initWidget();
	void initItem(QTreeWidgetItem *item, const QString &text, bool expanded);

	QTreeWidgetItem *mDisplayRoot;  // Takes ownership
	QTreeWidgetItem *mDevicesRoot;  // Takes ownership
	QTreeWidgetItem *mMotorsRoot;   // Takes ownership
	QTreeWidgetItem *mPhysicsRoot;  // Takes ownership
	QTreeWidgetItem *mParamsRoot;  // Takes ownership
	QTreeWidgetItem *mDisplayItem;  // Takes ownership
	QTreeWidgetItem *mDevicesItem;  // Takes ownership
	QTreeWidgetItem *mMotorsItem;   // Takes ownership
	QTreeWidgetItem *mPhysicsItem;  // Takes ownership
	QTreeWidgetItem *mParamsItem;  // Takes ownership
	QWidget *mDisplay;  // Doesn`t take ownership
	QWidget *mDevices;  // Doesn`t take ownership
	QWidget *mMotors;   // Doesn`t take ownership
	QWidget *mPhysics;  // Doesn`t take ownership
	QWidget *mParams;  // Doesn`t take ownership
};

}
}
