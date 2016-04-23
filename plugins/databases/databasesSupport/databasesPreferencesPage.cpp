/* Copyright 2014-2016 Anastasia Semenova
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

#include "databasesPreferencesPage.h"

namespace qReal {
namespace databasesSupport {

DatabasesPreferencesPage::DatabasesPreferencesPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::DatabasesPreferencesPage())
{
	mUi->setupUi(this);
	connect(mUi->dbmsBox, SIGNAL(currentTextChanged(QString)), this, SLOT(dbmsChanging(QString)));
	connect(mUi->codeGenerationModeBox, SIGNAL(currentTextChanged(QString)), this, SLOT(codeGenerationModeChanging(QString)));
	connect(mUi->codeGenerationFileButton, SIGNAL(clicked()), this, SLOT(browseCodeGenerationFilename()));
	connect(mUi->createDatabaseCheckBox, SIGNAL(clicked(bool)), mUi->databaseName, SLOT(setEnabled(bool)));
}

DatabasesPreferencesPage::~DatabasesPreferencesPage()
{
	delete mUi;
}

void DatabasesPreferencesPage::dbmsChanging(QString const &dbmsName)
{
	if (dbmsName == "SQLite") {
		mUi->createDatabaseCheckBox->setEnabled(true);
		emit dbmsChanged("Sqlite");
	}
	else if (dbmsName == "Microsoft SQL Server 2008") {
		mUi->createDatabaseCheckBox->setEnabled(true);
		emit dbmsChanged("SqlServer2008");
	}
	else if (dbmsName == "MySQL 5") {
		mUi->createDatabaseCheckBox->setEnabled(true);
		emit dbmsChanged("MySql5");
	}
	else if (dbmsName == "Microsoft Access") {
		mUi->createDatabaseCheckBox->setEnabled(false);
		emit dbmsChanged("MicrosoftAccess");
	}
	else if (dbmsName == "PostgreSQL") {
		mUi->createDatabaseCheckBox->setEnabled(true);
		emit dbmsChanged("PostgreSql");
	}

	mUi->databaseName->setEnabled(mUi->createDatabaseCheckBox->isEnabled()
			&& mUi->createDatabaseCheckBox->isChecked());
}

void DatabasesPreferencesPage::codeGenerationModeChanging(QString const &mode)
{
	if (mode == "CREATE TABLE")
		mUi->createDatabaseCheckBox->setEnabled(true);
	else
		mUi->createDatabaseCheckBox->setEnabled(false);

	mUi->databaseName->setEnabled(mUi->createDatabaseCheckBox->isEnabled()
			&& mUi->createDatabaseCheckBox->isChecked());
}

void DatabasesPreferencesPage::save()
{
}


void DatabasesPreferencesPage::restoreSettings()
{
}

void DatabasesPreferencesPage::browseCodeGenerationFilename()
{
	const QString path = utils::QRealFileDialog::getOpenFileName("FileForReverseEngineer"
			,this, tr("Open Directory")).replace("\\", "/");
	mUi->codeGenerationFilePath->setText(path);
}

QString DatabasesPreferencesPage::getCodeGenerationFilename() const
{
	return mUi->codeGenerationFilePath->toPlainText().toUtf8();
}

CodeGenerationMode DatabasesPreferencesPage::getCodeGenerationMode() const
{
	if (mUi->codeGenerationModeBox->currentText() == "CREATE TABLE")
		return CreateTable;
	else
		return AlterTable;
}

}
}
