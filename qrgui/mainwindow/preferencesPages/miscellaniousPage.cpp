#include "../../kernel/settingsManager.h"
#include <QFileDialog>

#include "preferencesPages/miscellaniousPage.h"
#include "ui_miscellaniousPage.h"

PreferencesMiscellaniousPage::PreferencesMiscellaniousPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesMiscellaniousPage)
{
	mUi->setupUi(this);

	connect(mUi->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));

	mUi->chaoticEditionCheckBox->setChecked(SettingsManager::instance()->value("ChaoticEdition", false).toBool());
	mUi->antialiasingCheckBox->setChecked(SettingsManager::instance()->value("Antialiasing", true).toBool());
	mUi->splashScreenCheckBox->setChecked(SettingsManager::instance()->value("Splashscreen", true).toBool());
	mUi->openGLCheckBox->setChecked(SettingsManager::instance()->value("OpenGL", true).toBool());
	mUi->squareLineModeCheckBox->setChecked(SettingsManager::instance()->value("SquareLine", false).toBool());

	mLastIconsetPath = SettingsManager::instance()->value("pathToImages", QDir::currentPath() + "/images/iconset1").toString();
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
	mUi->imagesPathEdit->setText(path.replace("\\", "/"));
}

void PreferencesMiscellaniousPage::save()
{
	SettingsManager::instance()->setValue("Splashscreen", mUi->splashScreenCheckBox->isChecked());
	SettingsManager::instance()->setValue("Antialiasing", mUi->antialiasingCheckBox->isChecked());
	SettingsManager::instance()->setValue("OpenGL", mUi->openGLCheckBox->isChecked());
	SettingsManager::instance()->setValue("SquareLine", mUi->squareLineModeCheckBox->isChecked());
	SettingsManager::instance()->setValue("ChaoticEdition", mUi->chaoticEditionCheckBox->isChecked());
	SettingsManager::instance()->setValue("pathToImages", mUi->imagesPathEdit->text());

	if (mLastIconsetPath != mUi->imagesPathEdit->text())
		emit iconsetChanged();
}
