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

#include "kitBase/devicesConfigurationWidget.h"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>

#include "kitBase/robotModel/robotModelInterface.h"

using namespace kitBase;
using namespace robotModel;

DevicesConfigurationWidget::DevicesConfigurationWidget(QWidget *parent, bool autosaveMode, bool compactMode)
	: QScrollArea(parent)
	, mAutosaveMode(autosaveMode)
	, mCompactMode(compactMode)
	, mSaving(false)
	, mRefreshing(false)
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setMinimumHeight(125);
	setFrameShape(QFrame::NoFrame);

	setWidgetResizable(true);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Background, Qt::transparent);
	setPalette(palette);
}

DevicesConfigurationWidget::~DevicesConfigurationWidget()
{
	qDeleteAll(mRobotModelConfigurers);
	mRobotModelConfigurers.clear();
	mConfigurers.clear();
}

void DevicesConfigurationWidget::loadRobotModels(QList<RobotModelInterface *> const &models)
{
	for (RobotModelInterface * const model : models) {
		const QString name = model->name();
		mRobotModels[name] = model;
		QWidget * const configurer = configurerForRobotModel(*model);
		mRobotModelConfigurers[name] = configurer;
	}
}

void DevicesConfigurationWidget::selectRobotModel(RobotModelInterface &robotModel)
{
	if (mCurrentModelType == robotModel.name() && mCurrentModelId == robotModel.robotId()) {
		return;
	}

	mCurrentModelType = robotModel.name();
	mCurrentModelId = robotModel.robotId();
	takeWidget();
	if (mRobotModels.contains(mCurrentModelType)) {
		setWidget(mRobotModelConfigurers[mCurrentModelType]);
		refresh();
	}
}

void DevicesConfigurationWidget::prependCustomWidget(RobotModelInterface &robotModel, QWidget * const widget)
{
	if (!widget) {
		return;
	}

	Q_ASSERT(mRobotModelConfigurers.contains(robotModel.name()));
	QVBoxLayout *layout = dynamic_cast<QVBoxLayout *>(mRobotModelConfigurers[robotModel.name()]->layout());
	Q_ASSERT(layout);
	layout->insertWidget(0, widget);
}

QWidget *DevicesConfigurationWidget::configurerForRobotModel(RobotModelInterface &robotModel)
{
	/// @todo: What if robot model has no configurable sensors?
	QWidget *result = new QWidget;
	QPalette palette = result->palette();
	palette.setColor(QPalette::Background, Qt::transparent);
	result->setPalette(palette);
	QVBoxLayout * const layout = new QVBoxLayout(result);
	layout->setContentsMargins(0, 0, 0, 0);
	QList<PortInfo> const configurablePorts = robotModel.configurablePorts();
	for (const PortInfo &port : configurablePorts) {
		layout->addLayout(initPort(robotModel.name(), port, robotModel.allowedDevices(port)));
	}

	return result;
}

QLayout *DevicesConfigurationWidget::initPort(const QString &robotModel
		, const PortInfo &port, const QList<DeviceInfo> &sensors)
{
	const QString labelText = mCompactMode ? tr("%1:") : tr("Port %1:");
	QLabel * const portLabel = new QLabel(labelText.arg(port.userFriendlyName()), this);
	QComboBox * const comboBox = new QComboBox(this);
	QPalette palette = comboBox->palette();
	palette.setColor(QPalette::Base, Qt::white);
	comboBox->setPalette(Qt::white);
	comboBox->setObjectName("Port " + port.name() + " DeviceConfig");
	comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
	comboBox->setProperty("robotModel", robotModel);
	comboBox->setProperty("port", QVariant::fromValue(port));
	mConfigurers << comboBox;
	comboBox->addItem(tr("Unused"), QVariant::fromValue(DeviceInfo()));
	for (const DeviceInfo &sensor : sensors) {
		comboBox->addItem(sensor.friendlyName(), QVariant::fromValue(sensor));
	}

	if (mAutosaveMode) {
		connect(comboBox, &QComboBox::currentTextChanged, this, &DevicesConfigurationWidget::save);
	}

	QHBoxLayout * const layout = new QHBoxLayout;
	layout->addWidget(portLabel);
	layout->addWidget(comboBox);
	layout->setStretch(0, 0);
	layout->setStretch(1, 1);

	return layout;
}

void DevicesConfigurationWidget::onDeviceConfigurationChanged(const QString &robotModel
		, const PortInfo &port, const DeviceInfo &sensor, Reason reason)
{
	Q_UNUSED(port)
	Q_UNUSED(sensor)
	Q_UNUSED(reason)

	// This method can be called when we did not accomplish processing all combo boxes during saving.
	// So ignoring such case.
	if (!mSaving && robotModel == mCurrentModelId) {
		refresh();
	}
}

void DevicesConfigurationWidget::refresh()
{
	mRefreshing = true;
	for (QComboBox * const box : mConfigurers) {
		const PortInfo port = box->property("port").value<PortInfo>();
		const DeviceInfo device = currentConfiguration(mCurrentModelId, port);
		if (device.isNull()) {
			box->setCurrentIndex(0);
		} else {
			for (int index = 0; index < box->count(); ++index) {
				if (box->itemData(index).value<DeviceInfo>().isA(device)) {
					box->setCurrentIndex(index);
					break;
				}
			}
		}
	}

	mRefreshing = false;
}

void DevicesConfigurationWidget::save()
{
	// Refreshing may affect combobox current index. No saving is needed then.
	if (mRefreshing) {
		return;
	}

	mSaving = true;
	for (QComboBox * const box : mConfigurers) {
		if (!box->isVisible()) {
			continue;
		}

		const QString robotModel = box->property("robotModel").toString();
		const PortInfo port = box->property("port").value<PortInfo>();
		const DeviceInfo device = box->itemData(box->currentIndex()).value<DeviceInfo>();
		if (robotModel == mCurrentModelType && currentConfiguration(mCurrentModelId, port) != device) {
			propagateChanges(port, device);
		}
	}

	mSaving = false;
}

void DevicesConfigurationWidget::propagateChanges(const PortInfo &port, const DeviceInfo &sensor)
{
	for (const QString &robotModelType : mRobotModels.keys()) {
		const RobotModelInterface *robotModel = mRobotModels[robotModelType];
		for (const PortInfo &otherPort : robotModel->configurablePorts()) {
			if (areConvertible(port, otherPort)) {
				if (sensor.isNull()) {
					deviceConfigurationChanged(robotModel->robotId(), otherPort, DeviceInfo(), Reason::userAction);
				} else {
					const DeviceInfo otherDevice = convertibleDevice(robotModel, otherPort, sensor);
					if (!otherDevice.isNull()) {
						deviceConfigurationChanged(robotModel->robotId(), otherPort, otherDevice, Reason::userAction);
					}
				}
			}
		}
	}
}

bool DevicesConfigurationWidget::areConvertible(const PortInfo &port1, const PortInfo &port2) const
{
	return (port1.name() == port2.name()
			|| port1.nameAliases().contains(port2.name())
			|| port2.nameAliases().contains(port1.name()))
			&& port1.direction() == port2.direction();
}

DeviceInfo DevicesConfigurationWidget::convertibleDevice(const RobotModelInterface *robotModel
		, const PortInfo &port, const DeviceInfo &device) const
{
	QList<DeviceInfo> const convertibleBases = robotModel->convertibleBases();
	for (const DeviceInfo &allowedDevice : robotModel->allowedDevices(port)) {
		for (const DeviceInfo &convertibleBase : convertibleBases) {
			if (device.isA(convertibleBase) && allowedDevice.isA(convertibleBase)) {
				return allowedDevice;
			}
		}
	}

	return DeviceInfo();
}
