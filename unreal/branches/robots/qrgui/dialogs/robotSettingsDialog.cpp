#include "robotSettingsDialog.h"
#include "ui_robotSettingsDialog.h"

#include <QtCore/QSettings>
#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport-1.2win-alpha/qextserialenumerator.h"

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

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

	QSettings settings("SPbSU", "QReal");
	QString const defaultPortName = settings.value("bluetoothPortName", "").toString();

	if (ports.isEmpty()) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->setText(tr("No COM ports found. If you have a Bluetooth connection with active virtual COM port, please enter its name. Example: COM3"));
		mUi->directInputComPortLabel->show();
		mUi->directInputComPortLineEdit->show();
		mUi->directInputComPortLineEdit->setText(defaultPortName);
	} else {
		mUi->comPortComboBox->show();
		mUi->directInputComPortLabel->hide();
		mUi->directInputComPortLineEdit->hide();

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

	QStringList sensorNames;
	sensorNames << tr("Unused") << tr("Touch sensor (boolean value)") << tr("Touch sensor (raw value)") << tr("Sonar sensor");

	mUi->port1ComboBox->addItems(sensorNames);
	mUi->port2ComboBox->addItems(sensorNames);
	mUi->port3ComboBox->addItems(sensorNames);
	mUi->port4ComboBox->addItems(sensorNames);

	sensorType::SensorTypeEnum const port1 = static_cast<sensorType::SensorTypeEnum>(settings.value("port1SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port2 = static_cast<sensorType::SensorTypeEnum>(settings.value("port2SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port3 = static_cast<sensorType::SensorTypeEnum>(settings.value("port3SensorType", "0").toInt());
	sensorType::SensorTypeEnum const port4 = static_cast<sensorType::SensorTypeEnum>(settings.value("port4SensorType", "0").toInt());

	mUi->port1ComboBox->setCurrentIndex(port1);
	mUi->port2ComboBox->setCurrentIndex(port2);
	mUi->port3ComboBox->setCurrentIndex(port3);
	mUi->port4ComboBox->setCurrentIndex(port4);
}

RobotSettingsDialog::~RobotSettingsDialog()
{
	delete mUi;
}

void RobotSettingsDialog::ok()
{
	accept();
	QSettings settings("SPbSU", "QReal");
	settings.setValue("bluetoothPortName", selectedPortName());
	settings.setValue("port1SensorType", selectedPort1Sensor());
	settings.setValue("port2SensorType", selectedPort2Sensor());
	settings.setValue("port3SensorType", selectedPort3Sensor());
	settings.setValue("port4SensorType", selectedPort4Sensor());
	close();
}

void RobotSettingsDialog::cancel()
{
	reject();
	close();
}

QString RobotSettingsDialog::selectedPortName() const
{
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
