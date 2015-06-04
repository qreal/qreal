#include "specifyPathToGeneratedCodeDialog.h"
#include "ui_specifyPathToGeneratedCodeDialog.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

using namespace qReal::gui;

SpecifyPathToGeneratedCodeDialog::SpecifyPathToGeneratedCodeDialog(
		qrRepo::LogicalRepoApi *metamodelRepoApi
		, QWidget *parent) :
	QDialog(parent)
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
			+ mUi->folderLineEdit->text() + tr(" directory will be removed. Continue?")) == QMessageBox::Yes) {
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
