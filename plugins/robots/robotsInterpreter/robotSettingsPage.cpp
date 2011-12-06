#include "robotSettingsPage.h"
#include "ui_robotSettingsPage.h"

#include "../../../qrkernel/settingsManager.h"
#include "../../../plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h"

using namespace qReal::interpreters::robots;

PreferencesRobotSettingsPage::PreferencesRobotSettingsPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesRobotSettingsPage),
	mIcon(":/icons/preferences/robot.png")
{
	mUi->setupUi(this);
	connect(mUi->nullModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(mUi->d2ModelRadioButton, SIGNAL(toggled(bool)), this, SLOT(activatedUnrealModel(bool)));
	connect(mUi->manualComPortCheckbox, SIGNAL(toggled(bool)), this, SLOT(manualComPortCheckboxChecked(bool)));

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
	QString const defaultPortName = SettingsManager::value("bluetoothPortName", "").toString();

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
		if (defaultIndex != -1) {
			mUi->comPortComboBox->setCurrentIndex(defaultIndex);
		}
	}

	if (SettingsManager::value("manualComPortCheckboxChecked", "false").toBool()) {
		mUi->manualComPortCheckbox->setChecked(true);
	}

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

	sensorType::SensorTypeEnum const port1 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::value("port1SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port2 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::value("port2SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port3 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::value("port3SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port4 = static_cast<sensorType::SensorTypeEnum>(SettingsManager::value("port4SensorType", "0").toInt());

	mUi->port1ComboBox->setCurrentIndex(port1);
	mUi->port2ComboBox->setCurrentIndex(port2);
	mUi->port3ComboBox->setCurrentIndex(port3);
	mUi->port4ComboBox->setCurrentIndex(port4);

	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(SettingsManager::value("robotModel", "1").toInt());
	initRobotModelType(typeOfRobotModel);

	QString const typeOfCommunication = SettingsManager::value("valueOfCommunication", "bluetooth").toString();
	initTypeOfCommunication(typeOfCommunication);
}

PreferencesRobotSettingsPage::~PreferencesRobotSettingsPage()
{
	delete mUi;
}

void PreferencesRobotSettingsPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange: {
		mUi->retranslateUi(this);

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

		mUi->port1ComboBox->clear();
		mUi->port2ComboBox->clear();
		mUi->port3ComboBox->clear();
		mUi->port4ComboBox->clear();
		mUi->port1ComboBox->addItems(sensorNames);
		mUi->port2ComboBox->addItems(sensorNames);
		mUi->port3ComboBox->addItems(sensorNames);
		mUi->port4ComboBox->addItems(sensorNames);
		break;
	}
	default:
		break;
	}
}

void PreferencesRobotSettingsPage::initRobotModelType(robotModelType::robotModelTypeEnum type)
{
	if (type == robotModelType::null) {
		mUi->nullModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else if (type == robotModelType::unreal) {
		mUi->d2ModelRadioButton->setChecked(true);
		activatedUnrealModel(true);
	} else {
		mUi->realModelRadioButton->setChecked(true);
	}
}

void PreferencesRobotSettingsPage::initTypeOfCommunication(QString type)
{
	if (type == "bluetooth")
		mUi->bluetoothRadioButton->setChecked(true);
	else
		mUi->usbRadioButton->setChecked(true);
}

robotModelType::robotModelTypeEnum PreferencesRobotSettingsPage::selectedRobotModel() const
{
	if (mUi->nullModelRadioButton->isChecked()) {
		return robotModelType::null;
	} else if (mUi->d2ModelRadioButton->isChecked()) {
		return robotModelType::unreal;
	} else {
		return robotModelType::real;
	}
}

QString PreferencesRobotSettingsPage::selectedCommunication() const
{
	return mUi->bluetoothRadioButton->isChecked()
			? "bluetooth"
			: "usb";
}

void PreferencesRobotSettingsPage::activatedUnrealModel(bool checked)
{
	mUi->bluetoothSettingsGroupBox->setEnabled(!checked);
}

void PreferencesRobotSettingsPage::manualComPortCheckboxChecked(bool state)
{
	SettingsManager::setValue("manualComPortCheckboxChecked", state);
	QString const defaultPortName = SettingsManager::value("bluetoothPortName", "").toString();

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

QString PreferencesRobotSettingsPage::selectedPortName() const
{
	if (!isVisible()) {
		return SettingsManager::value("bluetoothPortName", "").toString();
	}

	return mUi->comPortComboBox->isVisible()
			? mUi->comPortComboBox->currentText()
			: mUi->directInputComPortLineEdit->text();
}

sensorType::SensorTypeEnum PreferencesRobotSettingsPage::selectedPort1Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port1ComboBox->currentIndex());
}

sensorType::SensorTypeEnum PreferencesRobotSettingsPage::selectedPort2Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port2ComboBox->currentIndex());
}

sensorType::SensorTypeEnum PreferencesRobotSettingsPage::selectedPort3Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port3ComboBox->currentIndex());
}

sensorType::SensorTypeEnum PreferencesRobotSettingsPage::selectedPort4Sensor() const
{
	return static_cast<sensorType::SensorTypeEnum>(mUi->port4ComboBox->currentIndex());
}

void PreferencesRobotSettingsPage::save()
{
	SettingsManager::setValue("robotModel", selectedRobotModel());
	SettingsManager::setValue("bluetoothPortName", selectedPortName());
	SettingsManager::setValue("port1SensorType", selectedPort1Sensor());
	SettingsManager::setValue("port2SensorType", selectedPort2Sensor());
	SettingsManager::setValue("port3SensorType", selectedPort3Sensor());
	SettingsManager::setValue("port4SensorType", selectedPort4Sensor());
	SettingsManager::setValue("valueOfCommunication", selectedCommunication());
}

QIcon PreferencesRobotSettingsPage::getIcon()
{
	return mIcon;
}
