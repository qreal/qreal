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

#include "detailsTab.h"

using namespace twoDModel::view;

DetailsTab::DetailsTab(QWidget *parent)
	: QTreeWidget(parent)
	, mDisplayRoot(new QTreeWidgetItem(this))
	, mDevicesRoot(new QTreeWidgetItem(this))
	, mMotorsRoot(new QTreeWidgetItem(this))
	, mPhysicsRoot(new QTreeWidgetItem(this))
	, mDisplayItem(new QTreeWidgetItem(mDisplayRoot))
	, mDevicesItem(new QTreeWidgetItem(mDevicesRoot))
	, mMotorsItem(new QTreeWidgetItem(mMotorsRoot))
	, mPhysicsItem(new QTreeWidgetItem(mPhysicsRoot))
	, mDisplay(nullptr)
	, mDevices(nullptr)
	, mMotors(nullptr)
	, mPhysics(nullptr)
{
	initWidget();

	mDisplayRoot->setText(0, tr("Display"));
	mDevicesRoot->setText(0, tr("Ports configuration"));
	mMotorsRoot->setText(0, tr("Motors"));
	mPhysicsRoot->setText(0, tr("Physics"));

	mDisplayRoot->setExpanded(true);
	mDevicesRoot->setExpanded(true);
	mMotorsRoot->setExpanded(false);
	mPhysicsRoot->setExpanded(false);
}

DetailsTab::~DetailsTab()
{
}

void DetailsTab::initWidget()
{
	setHeaderHidden(true);
	setSelectionMode(NoSelection);
	setStyleSheet("QTreeWidget { background: transparent }");
	setAnimated(true);
}

void DetailsTab::setDisplay(QWidget *widget)
{
	mDisplay = widget;
	mDisplayRoot->setHidden(widget == nullptr);
	setItemWidget(mDisplayItem, 0, widget);
}

void DetailsTab::setDevicesConfigurer(QWidget *widget)
{
	mDevices = widget;
	setItemWidget(mDevicesItem, 0, widget);
}

void DetailsTab::setMotorsConfigurer(QWidget *widget)
{
	mMotors = widget;
	setItemWidget(mMotorsItem, 0, widget);
}

void DetailsTab::setPhysicsSettings(QWidget *widget)
{
	mPhysics = widget;
	setItemWidget(mPhysicsItem, 0, widget);
}

void DetailsTab::setDevicesSectionsVisible(bool visible)
{
	mDevices->setHidden(!visible);
	mMotors->setHidden(!visible);
}

void DetailsTab::setPhysicsSectionsVisible(bool visible)
{
	mPhysics->setHidden(!visible);
}
