#include "sensorsConfigurationWidget.h"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>

#include <qrkernel/settingsManager.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

using namespace interpreterCore::ui;
using namespace interpreterBase::robotModel;
using namespace qReal;

SensorsConfigurationWidget::SensorsConfigurationWidget(QWidget *parent, bool autosaveMode)
	: QWidget(parent)
	, mAutosaveMode(autosaveMode)
{
	refresh();
}

void SensorsConfigurationWidget::loadRobotModel(RobotModelInterface &robotModel)
{
	QString const robotModelId = robotModel.name();
	if (robotModelId == mRobotModelId) {
		return;
	}

	mRobotModelId = robotModelId;

	if (layout()) {
		/// @todo Clear it
		delete layout();
	}

	QVBoxLayout * const layout = new QVBoxLayout;
	setLayout(layout);
	QList<PortInfo> const configurablePorts = robotModel.configurablePorts();
	for (PortInfo const &port : configurablePorts) {
		layout->addLayout(initPort(port, robotModel.allowedDevices(port)));
	}

	refresh();
}

QLayout *SensorsConfigurationWidget::initPort(PortInfo const &port, QList<PluggableDeviceInfo> const &sensors)
{
	QLabel * const portLabel = new QLabel(tr("Port %1:").arg(port.name()), this);
	QComboBox * const comboBox = new QComboBox(this);
	comboBox->addItem(tr("Unused"), "");
	mBoxesToPorts[comboBox] = port;
	for (PluggableDeviceInfo const &sensor : sensors) {
		comboBox->addItem(sensor.friendlyName(), sensor.toString());
	}

	if (mAutosaveMode) {
		connect(comboBox, &QComboBox::currentTextChanged, this, &SensorsConfigurationWidget::save);
	}

	QHBoxLayout * const layout = new QHBoxLayout;
	layout->addWidget(portLabel);
	layout->addWidget(comboBox);
	layout->setStretch(0, 0);
	layout->setStretch(1, 1);


	return layout;
}

QString SensorsConfigurationWidget::settingsKey(PortInfo const &port) const
{
	return mRobotModelId + "SensorOnPort" + port.name();
}

void SensorsConfigurationWidget::refresh()
{
	for (QComboBox * const box : mBoxesToPorts.keys()) {
		QString const key = settingsKey(mBoxesToPorts[box]);
		QString const serializedDevice = SettingsManager::value(key).toString();
		if (serializedDevice.isEmpty()) {
			box->setCurrentIndex(0);
		} else {
			PluggableDeviceInfo const device = PluggableDeviceInfo::fromString(serializedDevice);
			box->setCurrentText(device.friendlyName());
		}
	}
}

void SensorsConfigurationWidget::save()
{
	for (QComboBox * const box : mBoxesToPorts.keys()) {
		QString const key = settingsKey(mBoxesToPorts[box]);
		SettingsManager::setValue(key, box->itemData(box->currentIndex()));
	}

	refreshSensorsConfiguration();
}
