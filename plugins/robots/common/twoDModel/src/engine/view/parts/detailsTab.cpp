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

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>

using namespace twoDModel::view;

DetailsTab::DetailsTab(QWidget *parent)
	: QTreeWidget(parent)
	, mDisplayRoot(new QTreeWidgetItem(this))
	, mDevicesRoot(new QTreeWidgetItem(this))
	, mMotorsRoot(new QTreeWidgetItem(this))
	, mPhysicsRoot(new QTreeWidgetItem(this))
	, mParamsRoot(new QTreeWidgetItem(this))
	, mDisplayItem(new QTreeWidgetItem(mDisplayRoot))
	, mDevicesItem(new QTreeWidgetItem(mDevicesRoot))
	, mMotorsItem(new QTreeWidgetItem(mMotorsRoot))
	, mPhysicsItem(new QTreeWidgetItem(mPhysicsRoot))
	, mParamsItem(new QTreeWidgetItem(mParamsRoot))
	, mDisplay(nullptr)
	, mDevices(nullptr)
	, mMotors(nullptr)
	, mPhysics(nullptr)
{
	initWidget();

	initItem(mDisplayRoot, tr("Display"), true);
	initItem(mDevicesRoot, tr("Ports configuration"), true);
	initItem(mMotorsRoot, tr("Motors"), false);
	initItem(mPhysicsRoot, tr("Physics"), false);
	initItem(mParamsRoot, tr("Model parameters"), false);
}

DetailsTab::~DetailsTab()
{
}

void DetailsTab::initWidget()
{
	setHeaderHidden(true);
	setSelectionMode(NoSelection);
	setIndentation(10);
	QPalette palette;
	palette.setColor(QPalette::Window, Qt::transparent);
	palette.setColor(QPalette::Base, Qt::transparent);
	setPalette(palette);
	setAnimated(true);
}

void DetailsTab::initItem(QTreeWidgetItem *item, const QString &text, bool expanded)
{
	QWidget * const widget = new QWidget(this);
	QVBoxLayout * const layout = new QVBoxLayout(widget);
	QLabel * const label = new QLabel(text, widget);
	label->setFrameStyle(QFrame::NoFrame);
	layout->addStretch();
	layout->addWidget(label);
	layout->addStretch();
	setItemWidget(item, 0, widget);
	item->setSizeHint(0, QSize(100, 50));
	item->setExpanded(expanded);
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
	mDevices->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

void DetailsTab::setMotorsConfigurer(QWidget *widget)
{
	mMotors = widget;
	setItemWidget(mMotorsItem, 0, widget);
	for (QWidget * const child : widget->findChildren<QComboBox *>()) {
		QPalette palette;
		palette.setColor(QPalette::Base, Qt::white);
		child->setPalette(palette);
	}
}

void DetailsTab::setPhysicsSettings(QWidget *widget)
{
	mPhysics = widget;
	setItemWidget(mPhysicsItem, 0, widget);
}

void DetailsTab::setParamsSettings(QWidget *widget)
{
	mParams = widget;
	setItemWidget(mParamsItem, 0, widget);
}

void DetailsTab::setDevicesSectionsVisible(bool visible)
{
	mDevicesRoot->setHidden(!visible);
}

void DetailsTab::setMotorsSectionsVisible(bool visible)
{
	mMotorsRoot->setHidden(!visible);
}

void DetailsTab::setPhysicsSectionsVisible(bool visible)
{
	mPhysicsRoot->setHidden(!visible);
}
