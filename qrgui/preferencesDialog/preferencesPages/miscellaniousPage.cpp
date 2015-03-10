#include "miscellaniousPage.h"
#include "ui_miscellaniousPage.h"

#include <qrkernel/settingsManager.h>
#include <qrutils/qRealFileDialog.h>

using namespace qReal;

PreferencesMiscellaniousPage::PreferencesMiscellaniousPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesMiscellaniousPage)
{
	mUi->setupUi(this);
	setObjectName("preferencesMiscellaniousPage");
	setWindowIcon(QIcon(":/preferencesDialog/images/miscellaneous.png"));

	connect(mUi->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));
	connect(mUi->toolbarSizeSlider, &QSlider::valueChanged
			, [=](int value ) { SettingsManager::setValue("toolbarSize", value); });

	restoreSettings();
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
	const QString path = utils::QRealFileDialog::getExistingDirectory("OpenImagesOnMiscellaniousPage"
			, this, tr("Open Directory")).replace("\\", "/");
	if (!path.isEmpty()) {
		mUi->imagesPathEdit->setText(path);
	}
}

void PreferencesMiscellaniousPage::save()
{
	SettingsManager::setValue("Splashscreen", mUi->splashScreenCheckBox->isChecked());
	SettingsManager::setValue("Antialiasing", mUi->antialiasingCheckBox->isChecked());

	SettingsManager::setValue("pathToImages", mUi->imagesPathEdit->text());
	SettingsManager::setValue("recentProjectsLimit", mUi->recentProjectsLimitSpinBox->value());

	SettingsManager::setValue("toolbarSize", mUi->toolbarSizeSlider->value());
}

void PreferencesMiscellaniousPage::restoreSettings()
{
	mUi->antialiasingCheckBox->setChecked(SettingsManager::value("Antialiasing").toBool());
	mUi->splashScreenCheckBox->setChecked(SettingsManager::value("Splashscreen").toBool());

	mUi->toolbarSizeSlider->setValue(SettingsManager::value("toolbarSize").toInt());

	mUi->imagesPathEdit->setText(SettingsManager::value("pathToImages").toString());
}
