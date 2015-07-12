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

#include "specifyPathToGeneratedCodeDialog.h"
#include "ui_specifyPathToGeneratedCodeDialog.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

using namespace qReal::gui;

SpecifyPathToGeneratedCodeDialog::SpecifyPathToGeneratedCodeDialog(
		qrRepo::LogicalRepoApi *metamodelRepoApi
		, QWidget *parent)
	: QDialog(parent)
	, mMetamodelRepoApi(metamodelRepoApi)
	, mUi(new Ui::SpecifyPathToGeneratedCodeDialog)
{
	mUi->setupUi(this);

	restoreSettings();

	this->setVisible(true);

	connect(mUi->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this
			, &SpecifyPathToGeneratedCodeDialog::saveSettings);

	connect(mUi->specifyFolderButton, &QPushButton::clicked, this
			, &SpecifyPathToGeneratedCodeDialog::specifyFolder);
}

SpecifyPathToGeneratedCodeDialog::~SpecifyPathToGeneratedCodeDialog()
{
	delete mUi;
}

QString SpecifyPathToGeneratedCodeDialog::currentPathToFolder() const
{
	return mUi->folderLineEdit->text();
}

QString SpecifyPathToGeneratedCodeDialog::currentFileName() const
{
	return mUi->fileNameLineEdit->text();
}

void SpecifyPathToGeneratedCodeDialog::restoreSettings()
{
	mUi->folderLineEdit->setText(mMetamodelRepoApi->metaInformation("PathToFolder").toString());
	mUi->fileNameLineEdit->setText(mMetamodelRepoApi->metaInformation("MainFileName").toString());
}

void SpecifyPathToGeneratedCodeDialog::saveSettings()
{
	mMetamodelRepoApi->setMetaInformation("PathToFolder", mUi->folderLineEdit->text());
	mMetamodelRepoApi->setMetaInformation("MainFileName", mUi->fileNameLineEdit->text());

	if (directoryCreated(mUi->folderLineEdit->text())) {
		emit pathsSpecified();
	}
}

void SpecifyPathToGeneratedCodeDialog::specifyFolder()
{
	const auto folderName = QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	mUi->folderLineEdit->setText(folderName);
}

bool SpecifyPathToGeneratedCodeDialog::directoryCreated(const QString &directory) const
{
	QDir dir;

	if (directoryRemoved(directory)) {
		dir.mkpath(directory);
		return true;
	}

	return false;
}

bool SpecifyPathToGeneratedCodeDialog::directoryRemoved(const QString &directory) const
{
	QDir dir(directory);

	if (!dir.exists() || dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot).isEmpty()) {
		return true;
	}

	if (QMessageBox::question(nullptr, tr("Warning"), tr("All files and folders from ")
			+ mUi->folderLineEdit->text() + tr(" directory will be removed. Continue?")) == QMessageBox::Yes)
	{
		clearDir(directory);

		return true;
	}

	return false;
}

void SpecifyPathToGeneratedCodeDialog::clearDir(const QString &path)
{
	if (path.isEmpty()) {
		return;
	}

	QDir dir(path);
	if (!dir.exists()) {
		return;
	}

	for (const auto &fileInfo : dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
		if (fileInfo.isDir()) {
			clearDir(fileInfo.filePath());
		} else {
			dir.remove(fileInfo.fileName());
		}
	}

	dir.rmdir(path);
}
