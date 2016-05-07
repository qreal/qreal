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

#include "generateSchemaWidget.h"
#include "ui_generateSchemaWidget.h"

namespace qReal {
namespace databasesSupport {

GenerateSchemaWidget::GenerateSchemaWidget(QWidget *parent) :
	QDialog(parent)
	, mUi(new Ui::GenerateSchemaWidget)
	, mDbms(QString("SQLite"))
	, mFilePath(QString(""))
{
	mUi->setupUi(this);
	this->setVisible(false);
	connect(mUi->dbmsBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateDbms(QString)));
	connect(mUi->reverseEngineerFileButton, SIGNAL(clicked()), this, SLOT(updateFilePath()));
}

GenerateSchemaWidget::~GenerateSchemaWidget()
{
	delete mUi;
}

QString GenerateSchemaWidget::getDbms() const
{
	if (mDbms == "SQLite")
		return QString("Sqlite");
	else
		return QString("MicrosoftAccess");
}

QString GenerateSchemaWidget::getFilePath() const
{
	return mFilePath;
}

void GenerateSchemaWidget::updateDbms(QString const &dbms)
{
	mDbms = dbms;
}

void GenerateSchemaWidget::updateFilePath()
{
	const QString path = utils::QRealFileDialog::getOpenFileName("FileForReverseEngineer"
			,this, tr("Open Directory")).replace("\\", "/");
	mUi->reverseEngineerFilePath->setText(path);
	mFilePath = path;
}

}
}
