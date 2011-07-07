#include <QSettings>

#include "preferencesPages/robotSettingsPage.h"
#include "ui_robotSettingsPage.h"
#include "../../thirdparty/qextserialport/src/qextserialenumerator.h"

using namespace qReal::interpreters::robots;

PreferencesRobotSettingsPage::PreferencesRobotSettingsPage(QWidget *parent) :
	PreferencesPage(parent),
	ui(new Ui::PreferencesRobotSettingsPage)
{
	ui->setupUi(this);

	connect(ui->nullModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(ui->d2ModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(ui->manualComPortCheckbox, SIGNAL(toggled(bool)), this, SLOT(manualComPortCheckboxChecked(bool)));

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
	QSettings settings("SPbSU", "QReal");
	QString const defaultPortName = settings.value("bluetoothPortName", "").toString();

	if (ports.isEmpty()) {
		ui->comPortComboBox->hide();
		ui->comPortLabel->hide();
		ui->manualComPortCheckbox->hide();
		ui->noComPortsFoundLabel->show();
		ui->directInputComPortLabel->show();
		ui->directInputComPortLineEdit->show();
		ui->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		ui->comPortComboBox->show();
		ui->comPortLabel->show();
		ui->manualComPortCheckbox->show();
		ui->directInputComPortLabel->hide();
		ui->directInputComPortLineEdit->hide();
		ui->noComPortsFoundLabel->hide();

		foreach (QextPortInfo info, ports) {
			QRegExp const portNameRegexp("COM\\d+", Qt::CaseInsensitive);
			if (portNameRegexp.indexIn(info.portName) != -1) {
				QString const portName = portNameRegexp.cap();
				ui->comPortComboBox->addItem(portName);
			}
		}
		int const defaultIndex = ui->comPortComboBox->findText(defaultPortName);
		if (defaultIndex != -1)
			ui->comPortComboBox->setCurrentIndex(defaultIndex);
	}

	if (settings.value("manualComPortCheckboxChecked", "false").toBool())
		ui->manualComPortCheckbox->setChecked(true);

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

	ui->port1ComboBox->addItems(sensorNames);
	ui->port2ComboBox->addItems(sensorNames);
	ui->port3ComboBox->addItems(sensorNames);
	ui->port4ComboBox->addItems(sensorNames);

	sensorType::SensorTypeEnum const port1 = static_cast<sensorType::SensorTypeEnum>(settings.value("port1SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port2 = static_cast<sensorType::SensorTypeEnum>(settings.value("port2SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port3 = static_cast<sensorType::SensorTypeEnum>(settings.value("port3SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port4 = static_cast<sensorType::SensorTypeEnum>(settings.value("port4SensorType", "0").toInt());

	ui->port1ComboBox->setCurrentIndex(port1);
	ui->port2ComboBox->setCurrentIndex(port2);
	ui->port3ComboBox->setCurrentIndex(port3);
	ui->port4ComboBox->setCurrentIndex(port4);

	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(settings.value("robotModel", "1").toInt());
	initRobotModelType(typeOfRobotModel);
}

PreferencesRobotSettingsPage::~PreferencesRobotSettingsPage()
{
	delete ui;
}

void PreferencesRobotSettingsPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesRobotSettingsPage::initRobotModelType(robotModelType::robotModelTypeEnum type)
{
	if (type == robotModelType::null) {
		ui->nullModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else if (type == robotModelType::unreal) {
		ui->d2ModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else
		ui->realModelRadioButton->setChecked(true);
}

robotModelType::robotModelTypeEnum PreferencesRobotSettingsPage::selectedRobotModel() const
{
	if (ui->nullModelRadioButton->isChecked())
		return robotModelType::null;
	else if (ui->d2ModelRadioButton->isChecked())
		return robotModelType::unreal;
	else
		return robotModelType::real;
}


void PreferencesRobotSettingsPage::activatedUnrealModel(bool checked)
{
	if (checked)
		ui->bluetoothSettingsGroupBox->setEnabled(false);
	else
		ui->bluetoothSettingsGroupBox->setEnabled(true);
}

void PreferencesRobotSettingsPage::manualComPortCheckboxChecked(bool state)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("manualComPortCheckboxChecked", state);
	QString const defaultPortName = settings.value("bluetoothPortName", "").toString();

	if (state) {
		ui->comPortComboBox->hide();
		ui->comPortLabel->hide();
		ui->directInputComPortLabel->show();
		ui->directInputComPortLineEdit->show();
		ui->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		ui->comPortComboBox->show();
		ui->comPortLabel->show();
		ui->directInputComPortLabel->hide();
		ui->directInputComPortLineEdit->hide();
		ui->noComPortsFoundLabel->hide();
	}
}

QString PreferencesRobotSettingsPage::selectedPortName() const
{
	if (!isVisible()) {
		QSettings settings("SPbSU", "QReal");
		return settings.value("bluetoothPortName", "").toString();
	}

	if (ui->comPortComboBox->isVisible())
		return ui->comPortComboBox->currentText();
	else
		return ui->directInputComPortLineEdit->text();
}

sensorType::SensorTypeEnum PreferencesRobotSettingsPage::selectedPort1Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(ui->port1ComboBox->currentIndex());
}

sensorType::SensorTypeEnum PreferencesRobotSettingsPage::selectedPort2Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(ui->port2ComboBox->currentIndex());
}

sensorType::SensorTypeEnum PreferencesRobotSettingsPage::selectedPort3Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(ui->port3ComboBox->currentIndex());
}

sensorType::SensorTypeEnum PreferencesRobotSettingsPage::selectedPort4Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(ui->port4ComboBox->currentIndex());
}

void PreferencesRobotSettingsPage::save()
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("robotModel", selectedRobotModel());
	settings.setValue("bluetoothPortName", selectedPortName());
	settings.setValue("port1SensorType", selectedPort1Sensor());
	settings.setValue("port2SensorType", selectedPort2Sensor());
	settings.setValue("port3SensorType", selectedPort3Sensor());
	settings.setValue("port4SensorType", selectedPort4Sensor());
}
