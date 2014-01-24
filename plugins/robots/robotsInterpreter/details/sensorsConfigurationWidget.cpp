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
	QStringList const sensorNames = SensorEnumerator::sensorNamesList();

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

void SensorsConfigurationWidget::save()
{
	sensorConfigurationChanged(
			enums::inputPort::port1
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(selectedPortSensor(*mUi->port1ComboBox))
			);


	sensorConfigurationChanged(
			enums::inputPort::port2
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(selectedPortSensor(*mUi->port2ComboBox))
			);

	sensorConfigurationChanged(
			enums::inputPort::port3
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(selectedPortSensor(*mUi->port3ComboBox))
			);

	sensorConfigurationChanged(
			enums::inputPort::port4
			, static_cast<robots::enums::sensorType::SensorTypeEnum>(selectedPortSensor(*mUi->port4ComboBox))
			);
}

void SensorsConfigurationWidget::onSensorConfigurationChanged(
		enums::inputPort::InputPortEnum port
		, enums::sensorType::SensorTypeEnum type
		)
{
	switch (port) {
	case enums::inputPort::port1:
		mUi->port1ComboBox->setCurrentIndex(type);
	case enums::inputPort::port2:
		mUi->port1ComboBox->setCurrentIndex(type);
	case enums::inputPort::port3:
		mUi->port1ComboBox->setCurrentIndex(type);
	case enums::inputPort::port4:
		mUi->port1ComboBox->setCurrentIndex(type);
	}
}

void SensorsConfigurationWidget::retranslateUi()
{
	mUi->retranslateUi(this);
	reinitValues();
}

enums::sensorType::SensorTypeEnum SensorsConfigurationWidget::selectedPortSensor(QComboBox const &comboBox) const
{
	return static_cast<enums::sensorType::SensorTypeEnum>(comboBox.currentIndex());
}
