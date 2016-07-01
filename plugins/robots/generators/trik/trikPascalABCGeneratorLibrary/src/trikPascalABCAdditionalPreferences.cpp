/* Copyright 2016 CyberTech Labs Ltd.
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

#include "trikPascalABCAdditionalPreferences.h"
#include "ui_trikPascalABCAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>

using namespace trik::pascalABC;
using namespace qReal;

TrikPascalABCAdditionalPreferences::TrikPascalABCAdditionalPreferences(const QString &robotName, QWidget *parent)
	: AdditionalPreferences(parent)
	, mRobotName(robotName)
	, mUi(new Ui::TrikPascalAbcAdditionalPreferences)
{
	mUi->setupUi(this);
}

TrikPascalABCAdditionalPreferences::~TrikPascalABCAdditionalPreferences()
{
	delete mUi;
}

void TrikPascalABCAdditionalPreferences::save()
{
	SettingsManager::setValue("PascalABCPath", mUi->pascalAbcLineEdit->text());
}

void TrikPascalABCAdditionalPreferences::restoreSettings()
{
	mUi->pascalAbcLineEdit->setText(SettingsManager::value("PascalABCPath").toString());
}

void TrikPascalABCAdditionalPreferences::onRobotModelChanged(
		kitBase::robotModel::RobotModelInterface * const robotModel)
{
	mUi->pascalAbcSettingsGroupBox->setVisible(robotModel->name() == mRobotName);
}
