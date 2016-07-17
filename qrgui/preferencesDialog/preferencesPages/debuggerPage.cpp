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

#include "debuggerPage.h"
#include "ui_debuggerPage.h"

#include <qrkernel/settingsManager.h>

using namespace qReal;

PreferencesDebuggerPage::PreferencesDebuggerPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesDebuggerPage)

{
	mUi->setupUi(this);
	setObjectName("preferencesDebuggerPage");
	setWindowIcon(QIcon(":/preferencesDialog/images/bug.png"));

	restoreSettings();
}

PreferencesDebuggerPage::~PreferencesDebuggerPage()
{
	delete mUi;
}

void PreferencesDebuggerPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesDebuggerPage::save()
{
	SettingsManager::setValue("debuggerTimeout", mUi->timeoutLineEdit->text());
	SettingsManager::setValue("debugColor", mUi->colorComboBox->currentText());
}

void PreferencesDebuggerPage::restoreSettings()
{
	mUi->timeoutLineEdit->setText(SettingsManager::value("debuggerTimeout").toString());
	mUi->colorComboBox->addItems(QColor::colorNames());
	QString curColor = SettingsManager::value("debugColor").toString();
	int curColorIndex = mUi->colorComboBox->findText(curColor);
	mUi->colorComboBox->setCurrentIndex(curColorIndex);
}
