/* Copyright 2017 QReal Research Group
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

#include "pioneerAdditionalPreferences.h"
#include "ui_pioneerAdditionalPreferences.h"

#include <QtWidgets/QFileDialog>

#include <qrkernel/settingsManager.h>
#include <pioneerKit/constants.h>

using namespace pioneer;
using namespace pioneer::lua;
using namespace qReal;

PioneerAdditionalPreferences::PioneerAdditionalPreferences(QWidget *parent)
	: AdditionalPreferences(parent)
	, mUi(new Ui::PioneerAdditionalPreferences)
{
	mUi->setupUi(this);

	restoreSettings();

	onCheckboxesChanged();

	connect(
			mUi->pythonPathBrowseButton
			, &QPushButton::clicked
			, this
			, &PioneerAdditionalPreferences::onPythonBrowseClick
	);

	connect(
			mUi->useControllerScriptCheckBox
			, &QCheckBox::stateChanged
			, this
			, &PioneerAdditionalPreferences::onCheckboxesChanged
	);

	connect(
			mUi->useComPortCheckBox
			, &QCheckBox::stateChanged
			, this
			, &PioneerAdditionalPreferences::onCheckboxesChanged
	);
}

PioneerAdditionalPreferences::~PioneerAdditionalPreferences()
{
	delete mUi;
}

void PioneerAdditionalPreferences::save()
{
	SettingsManager::setValue(settings::pioneerBaseStationIP, mUi->baseStationIpLineEdit->text());
	SettingsManager::setValue(settings::pioneerBaseStationPort, mUi->baseStationPortLineEdit->text());
	SettingsManager::setValue(settings::pioneerPythonPath, mUi->pythonPathLineEdit->text());

	SettingsManager::setValue(settings::pioneerUseControllerScript, mUi->useControllerScriptCheckBox->isChecked());
	SettingsManager::setValue(settings::pioneerUseComPort, mUi->useComPortCheckBox->isChecked());
	SettingsManager::setValue(settings::pioneerComPort, mUi->comPortLineEdit->text());

	emit settingsChanged();
}

void PioneerAdditionalPreferences::restoreSettings()
{
	mUi->baseStationIpLineEdit->setText(SettingsManager::value(settings::pioneerBaseStationIP).toString());
	mUi->baseStationPortLineEdit->setText(SettingsManager::value(settings::pioneerBaseStationPort).toString());
	mUi->pythonPathLineEdit->setText(SettingsManager::value(settings::pioneerPythonPath).toString());
	mUi->useControllerScriptCheckBox->setChecked(
			SettingsManager::value(settings::pioneerUseControllerScript).toBool()
	);

	mUi->useComPortCheckBox->setChecked(
			SettingsManager::value(settings::pioneerUseComPort).toBool()
	);

	mUi->comPortLineEdit->setText(
			SettingsManager::value(settings::pioneerComPort).toString()
	);
}

void PioneerAdditionalPreferences::onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel)
{
	Q_UNUSED(robotModel);
}

void PioneerAdditionalPreferences::onPythonBrowseClick()
{
#ifdef WIN32
	const QString filter = tr("Executable files (*.exe)");
#else
	const QString filter;
#endif

	const QString selectedPythonExecutable = QFileDialog::getOpenFileName(
			this
			, tr("Select Python 2.7 executable")
			, ""
			, filter);

	if (!selectedPythonExecutable.isEmpty()) {
		mUi->pythonPathLineEdit->setText(selectedPythonExecutable);
	}
}

void PioneerAdditionalPreferences::onCheckboxesChanged()
{
	const bool useControllerScript = mUi->useControllerScriptCheckBox->isChecked();
	mUi->pythonSettingsWidget->setVisible(useControllerScript);
	mUi->useComPortCheckBox->setVisible(useControllerScript);
	if (!mUi->useComPortCheckBox->isVisible()) {
		mUi->useComPortCheckBox->setChecked(false);
	}

	const bool useComPort = mUi->useComPortCheckBox->isChecked();

	mUi->baseStationIpLabel->setVisible(!useComPort);
	mUi->baseStationIpLineEdit->setVisible(!useComPort);
	mUi->baseStationPortLabel->setVisible(!useComPort);
	mUi->baseStationPortLineEdit->setVisible(!useComPort);

	mUi->comPortLabel->setVisible(useComPort);
	mUi->comPortLineEdit->setVisible(useComPort);
}
