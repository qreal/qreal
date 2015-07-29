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

#include "trikFSharpAdditionalPreferences.h"
#include "ui_trikFSharpAdditionalPreferences.h"

#include <qrkernel/settingsManager.h>

using namespace trik::fSharp;
using namespace qReal;

TrikFSharpAdditionalPreferences::TrikFSharpAdditionalPreferences(const QString &robotName, QWidget *parent)
	: AdditionalPreferences(parent)
	, mRobotName(robotName)
	, mUi(new Ui::TrikFSharpAdditionalPreferences)
{
	mUi->setupUi(this);
}

TrikFSharpAdditionalPreferences::~TrikFSharpAdditionalPreferences()
{
	delete mUi;
}

void TrikFSharpAdditionalPreferences::save()
{
	SettingsManager::setValue("FSharpPath", mUi->fSharpLineEdit->text());
}

void TrikFSharpAdditionalPreferences::restoreSettings()
{
	mUi->fSharpLineEdit->setText(SettingsManager::value("FSharpPath").toString());
}

void TrikFSharpAdditionalPreferences::onRobotModelChanged(kitBase::robotModel::RobotModelInterface * const robotModel)
{
	mUi->fSharpSettingsGroupBox->setVisible(robotModel->name() == mRobotName);
}
