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
#include <utils/widgets/comPortPicker.h>

using namespace ev3;
using namespace qReal;

Ev3AdditionalPreferences::Ev3AdditionalPreferences(QWidget *parent)
	: AdditionalPreferences(parent)
	, mUi(new Ui::Ev3AdditionalPreferences)
{
	mUi->setupUi(this);
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
	SettingsManager::setValue("Ev3ManualComPortCheckboxChecked", mUi->manualComPortCheckbox->isChecked());
	emit settingsChanged();
}

void Ev3AdditionalPreferences::restoreSettings()
{
	ui::ComPortPicker::populate(*mUi->comPortComboBox, "Ev3BluetoothPortName");

	if (mUi->comPortComboBox->count() == 0) {
		mUi->comPortComboBox->hide();
		mUi->comPortLabel->hide();
		mUi->manualComPortCheckbox->hide();
		mUi->noComPortsFoundLabel->show();
		mUi->directInputComPortLabel->show();
		mUi->directInputComPortLineEdit->show();
		mUi->directInputComPortLineEdit->setText(SettingsManager::value("Ev3BluetoothPortName").toString());
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
}

void Ev3AdditionalPreferences::onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel)
{
	mUi->bluetoothSettingsGroupBox->setVisible(robotModel->name().toLower().contains("bluetooth"));
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

QString Ev3AdditionalPreferences::selectedPortName() const
{
	return mUi->comPortComboBox->isVisible()
			? mUi->comPortComboBox->currentText()
			: mUi->directInputComPortLineEdit->text();
}
