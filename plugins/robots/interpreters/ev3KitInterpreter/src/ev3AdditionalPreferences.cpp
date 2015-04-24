/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "ev3AdditionalPreferences.h"
#include "ui_ev3AdditionalPreferences.h"

#include <qrkernel/settingsManager.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialenumerator.h>

using namespace ev3;
using namespace qReal;

Ev3AdditionalPreferences::Ev3AdditionalPreferences(const QString &realRobotName, QWidget *parent)
		: AdditionalPreferences(parent)
		, mUi(new Ui::Ev3AdditionalPreferences)
		, mRealRobotName(realRobotName)
{
	mUi->setupUi(this);
	connect(mUi->bluetoothRadioButton, &QRadioButton::toggled
			, mUi->bluetoothSettingsGroupBox, &QWidget::setVisible);
	connect(mUi->manualComPortCheckbox, &QCheckBox::toggled
			, this, &Ev3AdditionalPreferences::manualComPortCheckboxChecked);
}

Ev3AdditionalPreferences::~Ev3AdditionalPreferences()
{
	delete mUi;
}

void Ev3AdditionalPreferences::save()
{
	SettingsManager::setValue("Ev3BluetoothPortName", selectedPortName());
	SettingsManager::setValue("Ev3ValueOfCommunication", selectedCommunication());
	SettingsManager::setValue("Ev3ManualComPortCheckboxChecked", mUi->manualComPortCheckbox->isChecked());
	emit settingsChanged();
}

void Ev3AdditionalPreferences::restoreSettings()
{
	QList<QextPortInfo> const ports = QextSerialEnumerator::getPorts();
	const QString defaultPortName = SettingsManager::value("Ev3BluetoothPortName").toString();
	mUi->comPortComboBox->clear();

	for (const QextPortInfo &info : ports) {
		const QRegExp portNameRegexp("COM\\d+", Qt::CaseInsensitive);
		if (portNameRegexp.indexIn(info.portName) != -1) {
			const QString portName = portNameRegexp.cap();
			mUi->comPortComboBox->addItem(portName);
		}
	}

	const int defaultIndex = mUi->comPortComboBox->findText(defaultPortName);
	if (defaultIndex != -1) {
		mUi->comPortComboBox->setCurrentIndex(defaultIndex);
	}

	if (mUi->comPortComboBox->count() == 0) {
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
		mUi->manualComPortCheckbox->setChecked(false);
		mUi->manualComPortCheckbox->setChecked(SettingsManager::value("Ev3ManualComPortCheckboxChecked").toBool());
	}

	const QString typeOfCommunication = SettingsManager::value("Ev3ValueOfCommunication").toString();
	if (typeOfCommunication == "bluetooth") {
		mUi->bluetoothRadioButton->setChecked(true);
	} else if (typeOfCommunication == "usb") {
		mUi->usbRadioButton->setChecked(true);
	} else {
		mUi->bluetoothRadioButton->setChecked(false);
		mUi->usbRadioButton->setChecked(false);
	}
}

void Ev3AdditionalPreferences::onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel)
{
	const bool isReal = robotModel->name() == mRealRobotName;
	mUi->communicationTypeGroupBox->setVisible(isReal);
	mUi->bluetoothSettingsGroupBox->setVisible(mUi->bluetoothRadioButton->isChecked() && isReal);
}

void Ev3AdditionalPreferences::manualComPortCheckboxChecked(bool state)
{
	const QString defaultPortName = SettingsManager::value("Ev3BluetoothPortName").toString();

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

QString Ev3AdditionalPreferences::selectedCommunication() const
{
	return mUi->bluetoothRadioButton->isChecked() ? "bluetooth"
			: mUi->usbRadioButton->isChecked() ? "usb" : "";
}

QString Ev3AdditionalPreferences::selectedPortName() const
{
	return mUi->comPortComboBox->isVisible()
			? mUi->comPortComboBox->currentText()
			: mUi->directInputComPortLineEdit->text();
}
