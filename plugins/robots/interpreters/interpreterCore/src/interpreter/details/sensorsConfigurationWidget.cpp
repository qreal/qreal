#include "sensorsConfigurationWidget.h"

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>

#include <qrkernel/settingsManager.h>

using namespace interpreterCore::interpreter::details;

SensorsConfigurationWidget::SensorsConfigurationWidget(bool autosaveMode, QWidget *parent)
	: QWidget(parent)
	, mAutosaveMode(autosaveMode)
{
	refresh();
}

void SensorsConfigurationWidget::loadKit(QStringList const &ports, QStringList const &sensors)
{
	if (layout()) {
		delete layout();
		setLayout(nullptr);
	}

	QVBoxLayout * const layout = new QVBoxLayout;
	setLayout(layout);
	for (QString const &port : ports) {
		layout->addLayout(initPort(port, sensors));
	}
}

QLayout *SensorsConfigurationWidget::initPort(QString const &port, QStringList const &sensors)
{
	QHBoxLayout * const layout = new QHBoxLayout;
	QLabel * const portLabel = new QLabel(port + ":", this);
	QComboBox * const comboBox = new QComboBox(this);
	layout->addWidget(portLabel);
	layout->addWidget(comboBox);
	comboBox->addItems(sensors);
	if (mAutosaveMode) {
		connect(comboBox, &QComboBox::currentTextChanged, this, &SensorsConfigurationWidget::save);
	}

	return layout;
}

void SensorsConfigurationWidget::refresh()
{
//	robots::enums::sensorType::SensorTypeEnum const port1
//			= static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::value("port1SensorType").toInt());

//	robots::enums::sensorType::SensorTypeEnum const port2
//			= static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::value("port2SensorType").toInt());

//	robots::enums::sensorType::SensorTypeEnum const port3
//			= static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::value("port3SensorType").toInt());

//	robots::enums::sensorType::SensorTypeEnum const port4
//			= static_cast<robots::enums::sensorType::SensorTypeEnum>(SettingsManager::value("port4SensorType").toInt());

//	mUi->port1ComboBox->setCurrentIndex(port1);
//	mUi->port2ComboBox->setCurrentIndex(port2);
//	mUi->port3ComboBox->setCurrentIndex(port3);
//	mUi->port4ComboBox->setCurrentIndex(port4);
}

void SensorsConfigurationWidget::save()
{
//	SettingsManager::setValue("port1SensorType", selectedPort1Sensor());
//	SettingsManager::setValue("port2SensorType", selectedPort2Sensor());
//	SettingsManager::setValue("port3SensorType", selectedPort3Sensor());
//	SettingsManager::setValue("port4SensorType", selectedPort4Sensor());
	emit saved();
}

//qReal::interpreters::robots::enums::sensorType::SensorTypeEnum SensorsConfigurationWidget::selectedPort1Sensor() const
//{
//	return static_cast<robots::enums::sensorType::SensorTypeEnum>(mUi->port1ComboBox->currentIndex());
//}

//qReal::interpreters::robots::enums::sensorType::SensorTypeEnum SensorsConfigurationWidget::selectedPort2Sensor() const
//{
//	return static_cast<robots::enums::sensorType::SensorTypeEnum>(mUi->port2ComboBox->currentIndex());
//}

//qReal::interpreters::robots::enums::sensorType::SensorTypeEnum SensorsConfigurationWidget::selectedPort3Sensor() const
//{
//	return static_cast<robots::enums::sensorType::SensorTypeEnum>(mUi->port3ComboBox->currentIndex());
//}

//qReal::interpreters::robots::enums::sensorType::SensorTypeEnum SensorsConfigurationWidget::selectedPort4Sensor() const
//{
//	return static_cast<robots::enums::sensorType::SensorTypeEnum>(mUi->port4ComboBox->currentIndex());
//}
