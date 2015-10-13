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

#include <QtWidgets/QFileDialog>
#include <qrkernel/settingsManager.h>

#include "visualInterpreterPreferencesPage.h"
#include "ui_visualInterpreterPreferencePage.h"

using namespace qReal;

VisualInterpreterPreferencesPage::VisualInterpreterPreferencesPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::visualInterpreterPreferencesPage)
{
	setWindowIcon(QIcon(":/icons/preferences/bug.png"));
	mUi->setupUi(this);

	QString const binFolder = qApp->applicationDirPath();
	QDir binDir(binFolder);
	binDir.cdUp();
	SettingsManager::setValue("qrealSourcesLocation", binDir.absolutePath());

	mUi->qrealSourcesLineEdit->setText(SettingsManager::value("qrealSourcesLocation"
			, binFolder.mid(0, binFolder.lastIndexOf("/"))).toString());
	mUi->pythonPathLineEdit->setText(SettingsManager::value("pythonPath").toString());
	mUi->tempPathLineEdit->setText(SettingsManager::value("tempScriptPath", binFolder + "/temp.py").toString());
	mUi->genTimeoutSpinBox->setValue(SettingsManager::value("generationTimeout").toInt());

	connect(mUi->qrealSourcesPushButton, SIGNAL(clicked()), this, SLOT(setQRealSourcesLocation()));
	connect(mUi->pythonPathPushButton, SIGNAL(clicked()), this, SLOT(setPythonPath()));
	connect(mUi->tempPathPushButton, SIGNAL(clicked()), this, SLOT(setTempScriptPath()));
}

VisualInterpreterPreferencesPage::~VisualInterpreterPreferencesPage()
{
	delete mUi;
}

void VisualInterpreterPreferencesPage::setQRealSourcesLocation()
{
	QString const location = QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	mUi->qrealSourcesLineEdit->setText(location);
}

void VisualInterpreterPreferencesPage::setPythonPath()
{
	QString const path = QFileDialog::getOpenFileName(this, tr("Specify python path:"));
	mUi->pythonPathLineEdit->setText(path);
}

void VisualInterpreterPreferencesPage::setTempScriptPath()
{
	QString const path = QFileDialog::getOpenFileName(this, tr("Specify temp script file:"));
	mUi->tempPathLineEdit->setText(path);
}

void VisualInterpreterPreferencesPage::save()
{
	SettingsManager::setValue("qrealSourcesLocation", mUi->qrealSourcesLineEdit->text());
	SettingsManager::setValue("pythonPath", mUi->pythonPathLineEdit->text());
	SettingsManager::setValue("tempScriptPath", mUi->tempPathLineEdit->text());
	SettingsManager::setValue("generationTimeout", mUi->genTimeoutSpinBox->value());
}

void VisualInterpreterPreferencesPage::restoreSettings()
{
	mUi->qrealSourcesLineEdit->setText(SettingsManager::value("qrealSourcesLocation").toString());
}

