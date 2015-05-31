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

QString SpecifyPathToGeneratedCodeDialog::currentFileName() const
{
	return mUi->fileLineEdit->text();
}

QString SpecifyPathToGeneratedCodeDialog::currentPathToFolder() const
{
	return mUi->folderLineEdit->text();
}

void SpecifyPathToGeneratedCodeDialog::restoreSettings()
{
	mUi->folderLineEdit->setText(mMetamodelRepoApi->metaInformation("PathToFolder").toString());
	mUi->fileLineEdit->setText(mMetamodelRepoApi->metaInformation("MainFileName").toString());
}

void SpecifyPathToGeneratedCodeDialog::saveSettings()
{
	mMetamodelRepoApi->setMetaInformation("PathToFolder", mUi->folderLineEdit->text());
	mMetamodelRepoApi->setMetaInformation("MainFileName", mUi->fileLineEdit->text());

	emit pathsSpecified();
}

void SpecifyPathToGeneratedCodeDialog::specifyFolder()
{
	const auto folderName = QFileDialog::getExistingDirectory(this, tr("Specify directory:"));
	mUi->folderLineEdit->setText(folderName);
}
