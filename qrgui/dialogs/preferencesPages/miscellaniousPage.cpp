#include "../../../qrkernel/settingsManager.h"
#include <QFileDialog>

#include "miscellaniousPage.h"
#include "ui_miscellaniousPage.h"

PreferencesMiscellaniousPage::PreferencesMiscellaniousPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesMiscellaniousPage)
{
	mIcon = QIcon(":/icons/preferences/miscellaneous.png");
	mUi->setupUi(this);

	connect(mUi->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));

//	mUi->chaoticEditionCheckBox->setChecked(SettingsManager::value("ChaoticEdition").toBool());
	mUi->antialiasingCheckBox->setChecked(SettingsManager::value("Antialiasing").toBool());
	mUi->splashScreenCheckBox->setChecked(SettingsManager::value("Splashscreen").toBool());
	mUi->openGLCheckBox->setChecked(SettingsManager::value("OpenGL").toBool());
	mUi->squareLineModeCheckBox->setChecked(SettingsManager::value("SquareLine").toBool());

	mLastIconsetPath = SettingsManager::value("pathToImages", qApp->applicationDirPath() + "/images/iconset1").toString();
	mUi->imagesPathEdit->setText(mLastIconsetPath);
}

PreferencesMiscellaniousPage::~PreferencesMiscellaniousPage()
{
	delete mUi;
}

void PreferencesMiscellaniousPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesMiscellaniousPage::browseImagesPath()
{
	QString path = QFileDialog::getExistingDirectory(this, "Open Directory");
	if (!path.isEmpty()) {
		mUi->imagesPathEdit->setText(path.replace("\\", "/"));
	}
}

void PreferencesMiscellaniousPage::save()
{
	SettingsManager::setValue("Splashscreen", mUi->splashScreenCheckBox->isChecked());
	SettingsManager::setValue("Antialiasing", mUi->antialiasingCheckBox->isChecked());
	SettingsManager::setValue("OpenGL", mUi->openGLCheckBox->isChecked());
	SettingsManager::setValue("SquareLine", mUi->squareLineModeCheckBox->isChecked());
//	SettingsManager::setValue("ChaoticEdition", mUi->chaoticEditionCheckBox->isChecked());
	SettingsManager::setValue("pathToImages", mUi->imagesPathEdit->text());

	if (mLastIconsetPath != mUi->imagesPathEdit->text()) {
		emit iconsetChanged();
	}
}
