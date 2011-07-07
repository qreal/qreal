#include "robotSettingsDialog.h"
#include "ui_robotSettingsDialog.h"
#include "../kernel/exception/settingsManager.h"

#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"

using namespace qReal;
using namespace gui;
using namespace interpreters::robots;

RobotSettingsDialog::RobotSettingsDialog(QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::RobotSettingsDialog)
{
	mUi->setupUi(this);

	connect(mUi->okButton, SIGNAL(clicked()), this, SLOT(ok()));
	connect(mUi->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(mUi->nullModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(mUi->d2ModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(mUi->manualComPortCheckbox, SIGNAL(toggled(bool)), this, SLOT(manualComPortCheckboxChecked(bool)));

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

	QString const defaultPortName = SettingsManager::instance()->value("bluetoothPortName", "").toString();

	if (ports.isEmpty()) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->hide();
		mUi->manualComPortCheckbox->hide();
		mUi->noComPortsFoundLabel->show();
		mUi->directInputComPortLabel->show();
		mUi->directInputComPortLineEdit->show();
		mUi->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		mUi->comPortComboBox->show();
		mUi->comPortLabel->show();
		mUi->manualComPortCheckbox->show();
		mUi->directInputComPortLabel->hide();
		mUi->directInputComPortLineEdit->hide();
		mUi->noComPortsFoundLabel->hide();

		foreach (QextPortInfo info, ports) {
			QRegExp const portNameRegexp("COM\\d+", Qt::CaseInsensitive);
			if (portNameRegexp.indexIn(info.portName) != -1) {
				QString const portName = portNameRegexp.cap();
				mUi->comPortComboBox->addItem(portName);
			}
		}
		int const defaultIndex = mUi->comPortComboBox->findText(defaultPortName);
		if (defaultIndex != -1)
			mUi->comPortComboBox->setCurrentIndex(defaultIndex);
	}

	if (SettingsManager::instance()->value("manualComPortCheckboxChecked", "false").toBool())
		mUi->manualComPortCheckbox->setChecked(true);

	QStringList sensorNames;
	sensorNames << tr("Unused")
			<< tr("Touch sensor (boolean value)")
			<< tr("Touch sensor (raw value)")
			<< tr("Sonar sensor")
			<< tr("Color sensor (full colors)")
			<< tr("Color sensor (red)")
			<< tr("Color sensor (green)")
			<< tr("Color sensor (blue)")
			<< tr("Color sensor (passive)")
	;

	mUi->port1ComboBox->addItems(sensorNames);
	mUi->port2ComboBox->addItems(sensorNames);
	mUi->port3ComboBox->addItems(sensorNames);
	mUi->port4ComboBox->addItems(sensorNames);

	sensorType::SensorTypeEnum const port1 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port1SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port2 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port2SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port3 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port3SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port4 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::instance()->value("port4SensorType", "0").toInt());

	mUi->port1ComboBox->setCurrentIndex(port1);
	mUi->port2ComboBox->setCurrentIndex(port2);
	mUi->port3ComboBox->setCurrentIndex(port3);
	mUi->port4ComboBox->setCurrentIndex(port4);

	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::instance()->value("robotModel", "1").toInt());
	initRobotModelType(typeOfRobotModel);
}

RobotSettingsDialog::~RobotSettingsDialog()
{
	delete mUi;
}

void RobotSettingsDialog::ok()
{
	SettingsManager::instance()->setValue("bluetoothPortName", selectedPortName());
	SettingsManager::instance()->setValue("port1SensorType", selectedPort1Sensor());
	SettingsManager::instance()->setValue("port2SensorType", selectedPort2Sensor());
	SettingsManager::instance()->setValue("port3SensorType", selectedPort3Sensor());
	SettingsManager::instance()->setValue("port4SensorType", selectedPort4Sensor());
	SettingsManager::instance()->setValue("robotModel", selectedRobotModel());
	accept();
}

void RobotSettingsDialog::initRobotModelType(robotModelType::robotModelTypeEnum type)
{
	if (type == robotModelType::null) {
		mUi->nullModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else if (type == robotModelType::unreal) {
		mUi->d2ModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else
		mUi->realModelRadioButton->setChecked(true);
}

robotModelType::robotModelTypeEnum RobotSettingsDialog::selectedRobotModel() const
{
	if (mUi->nullModelRadioButton->isChecked())
		return robotModelType::null;
	else if (mUi->d2ModelRadioButton->isChecked())
		return robotModelType::unreal;
	else
		return robotModelType::real;
}

void RobotSettingsDialog::cancel()
{
	reject();
	close();
}

void RobotSettingsDialog::activatedUnrealModel(bool checked)
{
	if (checked)
		mUi->bluetoothSettingsGroupBox->setEnabled(false);
	else
		mUi->bluetoothSettingsGroupBox->setEnabled(true);
}

void RobotSettingsDialog::manualComPortCheckboxChecked(bool state)
{
	SettingsManager::instance()->setValue("manualComPortCheckboxChecked", state);
	QString const defaultPortName = SettingsManager::instance()->value("bluetoothPortName", "").toString();

	if (state) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->hide();
		mUi->directInputComPortLabel->show();
		mUi->directInputComPortLineEdit->show();
		mUi->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		mUi->comPortComboBox->show();
		mUi->comPortLabel->show();
		mUi->directInputComPortLabel->hide();
		mUi->directInputComPortLineEdit->hide();
		mUi->noComPortsFoundLabel->hide();
	}
}

QString RobotSettingsDialog::selectedPortName() const
{
	if (!isVisible()) {
		return SettingsManager::instance()->value("bluetoothPortName", "").toString();
	}

	if (mUi->comPortComboBox->isVisible())
		return mUi->comPortComboBox->currentText();
	else
		return mUi->directInputComPortLineEdit->text();
}

sensorType::SensorTypeEnum RobotSettingsDialog::selectedPort1Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port1ComboBox->currentIndex());
}

sensorType::SensorTypeEnum RobotSettingsDialog::selectedPort2Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port2ComboBox->currentIndex());
}

sensorType::SensorTypeEnum RobotSettingsDialog::selectedPort3Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port3ComboBox->currentIndex());
}

sensorType::SensorTypeEnum RobotSettingsDialog::selectedPort4Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port4ComboBox->currentIndex());
}
