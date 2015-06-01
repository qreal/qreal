#include "specifyPathToGeneratedCodeDialog.h"
#include "ui_specifyPathToGeneratedCodeDialog.h"

#include <QtWidgets/QFileDialog>

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

void SpecifyPathToGeneratedCodeDialog::restoreSettings()
{
	mUi->folderLineEdit->setText(mMetamodelRepoApi->metaInformation("PathToFolder").toString());
}

void SpecifyPathToGeneratedCodeDialog::saveSettings()
{
	mMetamodelRepoApi->setMetaInformation("PathToFolder", mUi->folderLineEdit->text());

	createDirectory(mUi->folderLineEdit->text());

	emit pathsSpecified();
}

void SpecifyPathToGeneratedCodeDialog::specifyFolder()
{
	const auto folderName = QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	mUi->folderLineEdit->setText(folderName);
}

void SpecifyPathToGeneratedCodeDialog::createDirectory(const QString &directory)
{
	QDir dir;
	clearDir(directory);
	dir.mkpath(directory);
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
