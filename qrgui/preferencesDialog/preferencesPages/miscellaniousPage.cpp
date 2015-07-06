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

#include "miscellaniousPage.h"
#include "ui_miscellaniousPage.h"

#include <qrkernel/settingsManager.h>
#include <qrutils/qRealFileDialog.h>

using namespace qReal;

PreferencesMiscellaniousPage::PreferencesMiscellaniousPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesMiscellaniousPage)
{
	mUi->setupUi(this);
	setObjectName("preferencesMiscellaniousPage");
	setWindowIcon(QIcon(":/preferencesDialog/images/miscellaneous.png"));

	connect(mUi->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));
	connect(mUi->toolbarSizeSlider, &QSlider::valueChanged
			, [=](int value ) { SettingsManager::setValue("toolbarSize", value); });

	restoreSettings();
}

PreferencesMiscellaniousPage::~PreferencesMiscellaniousPage()
{
	delete mUi;
}

void PreferencesMiscellaniousPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesMiscellaniousPage::browseImagesPath()
{
	const QString path = utils::QRealFileDialog::getExistingDirectory("OpenImagesOnMiscellaniousPage"
			, this, tr("Open Directory")).replace("\\", "/");
	if (!path.isEmpty()) {
		mUi->imagesPathEdit->setText(path);
	}
}

void PreferencesMiscellaniousPage::save()
{
	SettingsManager::setValue("Splashscreen", mUi->splashScreenCheckBox->isChecked());
	SettingsManager::setValue("Antialiasing", mUi->antialiasingCheckBox->isChecked());

	SettingsManager::setValue("pathToImages", mUi->imagesPathEdit->text());
	SettingsManager::setValue("recentProjectsLimit", mUi->recentProjectsLimitSpinBox->value());

	SettingsManager::setValue("toolbarSize", mUi->toolbarSizeSlider->value());
}

void PreferencesMiscellaniousPage::restoreSettings()
{
	mUi->antialiasingCheckBox->setChecked(SettingsManager::value("Antialiasing").toBool());
	mUi->splashScreenCheckBox->setChecked(SettingsManager::value("Splashscreen").toBool());

	mUi->toolbarSizeSlider->setValue(SettingsManager::value("toolbarSize").toInt());

	mUi->imagesPathEdit->setText(SettingsManager::value("pathToImages").toString());
}
