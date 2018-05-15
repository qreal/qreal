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
}

PioneerAdditionalPreferences::~PioneerAdditionalPreferences()
{
	delete mUi;
}

void PioneerAdditionalPreferences::save()
{
	SettingsManager::setValue(settings::pioneerBaseStationIP, mUi->baseStationIpLineEdit->text());
	SettingsManager::setValue(settings::pioneerBaseStationPort, mUi->baseStationPortLineEdit->text());

	emit settingsChanged();
}

void PioneerAdditionalPreferences::restoreSettings()
{
	mUi->baseStationIpLineEdit->setText(SettingsManager::value(settings::pioneerBaseStationIP).toString());
	mUi->baseStationPortLineEdit->setText(SettingsManager::value(settings::pioneerBaseStationPort).toString());
}

void PioneerAdditionalPreferences::onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel)
{
	Q_UNUSED(robotModel);
}
