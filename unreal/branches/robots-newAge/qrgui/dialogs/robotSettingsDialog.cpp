#include "robotSettingsDialog.h"
#include "ui_robotSettingsDialog.h"

#include <QtCore/QSettings>
#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport-1.2win-alpha/qextserialenumerator.h"

using namespace qReal;
using namespace gui;

RobotSettingsDialog::RobotSettingsDialog(QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::RobotSettingsDialog)
{
	mUi->setupUi(this);

	connect(mUi->okButton, SIGNAL(clicked()), this, SLOT(ok()));
	connect(mUi->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

	QSettings settings("SPbSU", "QReal");

	if (ports.isEmpty()) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->setText(tr("No COM ports found"));
	} else {
		qDebug() << "List of ports:";
		foreach (QextPortInfo info, ports) {
			qDebug() << "port name:" << info.portName;
			qDebug() << "friendly name:" << info.friendName;
			qDebug() << "physical name:" << info.physName;
			qDebug() << "enumerator name:" << info.enumName;
			qDebug() << "===================================";
			mUi->comPortComboBox->addItem(info.portName.left(4));
		}
		QString const defaultPortName = settings.value("bluetoothPortName", "").toString();
		int const defaultIndex = mUi->comPortComboBox->findText(defaultPortName);
		if (defaultIndex != -1)
			mUi->comPortComboBox->setCurrentIndex(defaultIndex);
	}

	QStringList sensorNames;
	sensorNames << "Unused" << "Touch sensor (boolean value)" << "Touch sensor (raw value)";

	mUi->port1ComboBox->addItems(sensorNames);
	mUi->port2ComboBox->addItems(sensorNames);
	mUi->port3ComboBox->addItems(sensorNames);
	mUi->port4ComboBox->addItems(sensorNames);

	interpreters::robots::SensorType::SensorType const port1 = static_cast<interpreters::robots::SensorType::SensorType>(settings.value("port1SensorType", "0").toInt());
	interpreters::robots::SensorType::SensorType const port2 = static_cast<interpreters::robots::SensorType::SensorType>(settings.value("port2SensorType", "0").toInt());
	interpreters::robots::SensorType::SensorType const port3 = static_cast<interpreters::robots::SensorType::SensorType>(settings.value("port3SensorType", "0").toInt());
	interpreters::robots::SensorType::SensorType const port4 = static_cast<interpreters::robots::SensorType::SensorType>(settings.value("port4SensorType", "0").toInt());

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
	setResult(QDialog::Accepted);
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
	setResult(QDialog::Rejected);
	close();
}

QString RobotSettingsDialog::selectedPortName() const
{
	return mUi->comPortComboBox->currentText();
}

interpreters::robots::SensorType::SensorType RobotSettingsDialog::selectedPort1Sensor() const
{
	return static_cast<interpreters::robots::SensorType::SensorType>(mUi->port1ComboBox->currentIndex());
}

interpreters::robots::SensorType::SensorType RobotSettingsDialog::selectedPort2Sensor() const
{
	return static_cast<interpreters::robots::SensorType::SensorType>(mUi->port2ComboBox->currentIndex());
}

interpreters::robots::SensorType::SensorType RobotSettingsDialog::selectedPort3Sensor() const
{
	return static_cast<interpreters::robots::SensorType::SensorType>(mUi->port3ComboBox->currentIndex());
}

interpreters::robots::SensorType::SensorType RobotSettingsDialog::selectedPort4Sensor() const
{
	return static_cast<interpreters::robots::SensorType::SensorType>(mUi->port4ComboBox->currentIndex());
}
