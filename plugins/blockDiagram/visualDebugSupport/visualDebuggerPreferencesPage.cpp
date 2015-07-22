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

#include "visualDebuggerPreferencesPage.h"
#include "ui_visualDebuggerPreferencesPage.h"

#include <qrkernel/settingsManager.h>

using namespace qReal;

VisualDebuggerPreferencesPage::VisualDebuggerPreferencesPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::VisualDebuggerPreferencesPage)
{
	setWindowIcon(QIcon(":/icons/preferences/bug.png"));

	mUi->setupUi(this);

	restoreSettings();

	connect(mUi->builderPathPushButton, SIGNAL(clicked()), this, SLOT(setBuilderPath()));
	connect(mUi->debuggerPathPushButton, SIGNAL(clicked()), this, SLOT(setDebuggerPath()));
	connect(mUi->visDebWorkDirPushButton, SIGNAL(clicked()), this, SLOT(setWorkDir()));
}

VisualDebuggerPreferencesPage::~VisualDebuggerPreferencesPage()
{
	delete mUi;
}

QString VisualDebuggerPreferencesPage::choosePath(bool isFolder)
{
	if (isFolder) {
		return QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	}

	return QFileDialog::getOpenFileName(this, tr("Specify file:"));
}

void VisualDebuggerPreferencesPage::putTextInLineEdit(QLineEdit *lineEdit, QString const &text)
{
	if (text != "") {
		lineEdit->setText(text);
	}
}

void VisualDebuggerPreferencesPage::setBuilderPath()
{
	putTextInLineEdit(mUi->builderPathLineEdit, choosePath(false));
}

void VisualDebuggerPreferencesPage::setDebuggerPath()
{
	putTextInLineEdit(mUi->debuggerPathLineEdit, choosePath(false));
}

void VisualDebuggerPreferencesPage::setWorkDir()
{
	putTextInLineEdit(mUi->visDebWorkDirLineEdit, choosePath(true));
}

void VisualDebuggerPreferencesPage::save()
{
	SettingsManager::setValue("debugWorkingDirectory", mUi->visDebWorkDirLineEdit->text());
	SettingsManager::setValue("codeFileName", mUi->codeFileNameLineEdit->text());
	SettingsManager::setValue("buildedFileName", mUi->buildedFileNameLineEdit->text());
	SettingsManager::setValue("builderPath", mUi->builderPathLineEdit->text());
	SettingsManager::setValue("debuggerPath", mUi->debuggerPathLineEdit->text());
}

void VisualDebuggerPreferencesPage::restoreSettings()
{
	mUi->builderPathLineEdit->setText(SettingsManager::value("builderPath").toString());
	mUi->buildedFileNameLineEdit->setText(SettingsManager::value("buildedFileName").toString());
	mUi->codeFileNameLineEdit->setText(SettingsManager::value("codeFileName").toString());
	mUi->visDebWorkDirLineEdit->setText(SettingsManager::value("debugWorkingDirectory").toString());
	mUi->debuggerPathLineEdit->setText(SettingsManager::value("debuggerPath").toString());
}
