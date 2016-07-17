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

#include <qrkernel/settingsManager.h>
#include "compilerPage.h"
#include "ui_compilerPage.h"

using namespace metaEditor;
using namespace qReal;

PreferencesCompilerPage::PreferencesCompilerPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesCompilerPage)
{
	setWindowIcon(QIcon(":/icons/tools.png"));
	mUi->setupUi(this);

	connect(mUi->linuxButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(mUi->windowsButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(mUi->otherButton, SIGNAL(clicked()), this, SLOT(changeSystem()));

	restoreSettings();
}

PreferencesCompilerPage::~PreferencesCompilerPage()
{
	delete mUi;
}

void PreferencesCompilerPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesCompilerPage::changeSystem()
{
	if (mUi->linuxButton->isChecked()) {
		initCompilersSettings("qmake", "make", "so", "lib");
	}
	if (mUi->windowsButton->isChecked()) {
		initCompilersSettings("qmake", "mingw32-make", "dll", "");
	}
	if (mUi->otherButton->isChecked()) {
		mUi->compilerSettingsWidget->setEnabled(true);
	}
}

void PreferencesCompilerPage::initCompilersSettings(QString const &pathToQmake,
		QString const &pathToMake, QString const &pluginExtension, QString const &prefix)
{
	mUi->pathToQmake->setText(pathToQmake);
	mUi->pathToMake->setText(pathToMake);
	mUi->pluginExtension->setText(pluginExtension);
	mUi->prefix->setText(prefix);
	mUi->compilerSettingsWidget->setEnabled(false);
}

void PreferencesCompilerPage::save()
{
	SettingsManager::setValue("windowsButton", mUi->windowsButton->isChecked());
	SettingsManager::setValue("linuxButton", mUi->linuxButton->isChecked());
	SettingsManager::setValue("otherButton", mUi->otherButton->isChecked());
	SettingsManager::setValue("pathToQmake", mUi->pathToQmake->text());
	SettingsManager::setValue("pathToMake", mUi->pathToMake->text());
	SettingsManager::setValue("pluginExtension", mUi->pluginExtension->text());
	SettingsManager::setValue("prefix", mUi->prefix->text());
}

void PreferencesCompilerPage::restoreSettings()
{
	mUi->windowsButton->setChecked(SettingsManager::value("windowsButton").toBool());
	mUi->linuxButton->setChecked(SettingsManager::value("linuxButton").toBool());
	mUi->otherButton->setChecked(SettingsManager::value("otherButton").toBool());
	mUi->pathToQmake->setText(SettingsManager::value("pathToQmake").toString());
	mUi->pathToMake->setText(SettingsManager::value("pathToMake").toString());
	mUi->pluginExtension->setText(SettingsManager::value("pluginExtension").toString());
	mUi->prefix->setText(SettingsManager::value("prefix").toString());
}
