#include <utils/sensorsConfigurationWidget.h>
#include <qrkernel/settingsManager.h>

using namespace utils;

SensorsConfigurationWidget::SensorsConfigurationWidget(bool autosaveMode, QWidget *parent)
	: QWidget(parent)
{
	reinitValues();
	refresh();
	if (autosaveMode) {
		startChangesListening();
	}
}

void SensorsConfigurationWidget::startChangesListening()
{
//	connect(mUi->port1ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
//	connect(mUi->port2ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
//	connect(mUi->port3ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
//	connect(mUi->port4ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
}

void SensorsConfigurationWidget::reinitValues()
{
//	QStringList const sensorNames = SensorEnumerator::sensorNamesList();

//	mUi->port1ComboBox->clear();
//	mUi->port2ComboBox->clear();
//	mUi->port3ComboBox->clear();
//	mUi->port4ComboBox->clear();
//	mUi->port1ComboBox->addItems(sensorNames);
//	mUi->port2ComboBox->addItems(sensorNames);
//	mUi->port3ComboBox->addItems(sensorNames);
//	mUi->port4ComboBox->addItems(sensorNames);
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
