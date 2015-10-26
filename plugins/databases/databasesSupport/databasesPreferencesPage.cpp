/* Copyright 2014-2015 Anastasia Semenova
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

}

DatabasesPreferencesPage::~DatabasesPreferencesPage()
{
	delete mUi;
}

void DatabasesPreferencesPage::dbmsChanging(QString const &dbmsName)
{
	if (dbmsName == "SQLite")
		emit dbmsChanged("sqlite");
	else if (dbmsName == "Microsoft SQL Server 2008")
		emit dbmsChanged("sqlServer2008");
	else if (dbmsName == "MySQL 5")
		emit dbmsChanged("mySql5");
	else if (dbmsName == "Microsoft Access")
		emit dbmsChanged("microsoftAccess");
}

void DatabasesPreferencesPage::changeReverseEngineerFilePath(QString const &path)
{
	//mUi->reverseEngineerFilePath->setText(path);
}

void DatabasesPreferencesPage::save()
{
}


void DatabasesPreferencesPage::restoreSettings()
{
}

void DatabasesPreferencesPage::browseFileForReverseEngineer()
{
	//const QString path = utils::QRealFileDialog::getOpenFileName("FileForReverseEngineer"
			//,this, tr("Open Directory")).replace("\\", "/");
	//mUi->reverseEngineerFilePath->setText(path);
}

const QString DatabasesPreferencesPage::getReverseEngineerFilename() const
{
	return QString();
//	QString const &str = mUi->reverseEngineerFilePath->toPlainText();
//	return str;
}

bool DatabasesPreferencesPage::needToOpenFileAfterGeneration() const
{
	return mUi->codeGenerationCheckBox->isChecked();
}
}
}
