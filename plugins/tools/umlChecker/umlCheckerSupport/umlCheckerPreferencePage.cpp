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

#include <QFileDialog>

#include "umlCheckerPreferencePage.h"
#include "ui_umlCheckerPreferencePage.h"

#include <qrkernel/settingsManager.h>

using namespace qReal;

UmlCheckerPreferencePage::UmlCheckerPreferencePage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::umlCheckerPreferencesPage)
{
	mUi->setupUi(this);

	connect(mUi->qrealSourcesPushButton, SIGNAL(clicked()), this, SLOT(setQRealSourcesLocation()));
}

UmlCheckerPreferencePage::~UmlCheckerPreferencePage()
{
	delete mUi;
}

void UmlCheckerPreferencePage::setQRealSourcesLocation()
{
	QString location = QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	mUi->qrealSourcesLineEdit->setText(location);
}

void UmlCheckerPreferencePage::save()
{
	SettingsManager::setValue("qrealSourcesLocation", mUi->qrealSourcesLineEdit->text());
}

void UmlCheckerPreferencePage::restoreSettings()
{
	mUi->qrealSourcesLineEdit->setText(SettingsManager::value("qrealSourcesLocation").toString());
}

void UmlCheckerPreferencePage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}
