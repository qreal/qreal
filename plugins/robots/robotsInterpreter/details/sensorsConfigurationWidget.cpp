#include "sensorsConfigurationWidget.h"
#include "ui_sensorsConfigurationWidget.h"
#include "../../../../qrkernel/settingsManager.h"

using namespace qReal::interpreters::robots;
using namespace qReal::interpreters::robots::details;

SensorsConfigurationWidget::SensorsConfigurationWidget(bool autosaveMode, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::SensorsConfigurationWidget)
{
	mUi->setupUi(this);
	reinitValues();
	refresh();
	if (autosaveMode) {
		startChangesListening();
	}
}

SensorsConfigurationWidget::~SensorsConfigurationWidget()
{
	delete mUi;
}

void SensorsConfigurationWidget::startChangesListening()
{
	connect(mUi->port1ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
	connect(mUi->port2ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
	connect(mUi->port3ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
	connect(mUi->port4ComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
}

void SensorsConfigurationWidget::reinitValues()
{
	QStringList sensorNames;
	sensorNames << tr("Unused")
			<< tr("Touch sensor (boolean value)")
			<< tr("Touch sensor (raw value)")
			<< tr("Sonar sensor")
			<< tr("Light sensor")
			<< tr("Color sensor (full colors)")
			<< tr("Color sensor (red)")
			<< tr("Color sensor (green)")
			<< tr("Color sensor (blue)")
			<< tr("Color sensor (passive)")
			<< tr("Sound sensor")
			<< tr("gyroscope(passive)")
			<< tr("aks(passive)")
	;

	mUi->port1ComboBox->clear();
	mUi->port2ComboBox->clear();
	mUi->port3ComboBox->clear();
	mUi->port4ComboBox->clear();
	mUi->port1ComboBox->addItems(sensorNames);
	mUi->port2ComboBox->addItems(sensorNames);
	mUi->port3ComboBox->addItems(sensorNames);
	mUi->port4ComboBox->addItems(sensorNames);
}


void SensorsConfigurationWidget::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange: {
		retranslateUi();
		break;
	}
	default:
		break;
	}
}

void SensorsConfigurationWidget::refresh()
{
	sensorType::SensorTypeEnum const port1 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::value("port1SensorType").toInt());
	sensorType::SensorTypeEnum const port2 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::value("port2SensorType").toInt());
	sensorType::SensorTypeEnum const port3 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::value("port3SensorType").toInt());
	sensorType::SensorTypeEnum const port4 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::value("port4SensorType").toInt());

	mUi->port1ComboBox->setCurrentIndex(port1);
	mUi->port2ComboBox->setCurrentIndex(port2);
	mUi->port3ComboBox->setCurrentIndex(port3);
	mUi->port4ComboBox->setCurrentIndex(port4);
}

void SensorsConfigurationWidget::save()
{
	SettingsManager::setValue("port1SensorType", selectedPort1Sensor());
	SettingsManager::setValue("port2SensorType", selectedPort2Sensor());
	SettingsManager::setValue("port3SensorType", selectedPort3Sensor());
	SettingsManager::setValue("port4SensorType", selectedPort4Sensor());
	emit saved();
}

void SensorsConfigurationWidget::retranslateUi()
{
	mUi->retranslateUi(this);
	reinitValues();
}

sensorType::SensorTypeEnum SensorsConfigurationWidget::selectedPort1Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port1ComboBox->currentIndex());
}

sensorType::SensorTypeEnum SensorsConfigurationWidget::selectedPort2Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port2ComboBox->currentIndex());
}

sensorType::SensorTypeEnum SensorsConfigurationWidget::selectedPort3Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port3ComboBox->currentIndex());
}

sensorType::SensorTypeEnum SensorsConfigurationWidget::selectedPort4Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port4ComboBox->currentIndex());
}
