#include "miscellaniousPage.h"
#include "ui_miscellaniousPage.h"

#include <QtWidgets/QFileDialog>

#include <qrkernel/settingsManager.h>

using namespace qReal;

PreferencesMiscellaniousPage::PreferencesMiscellaniousPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesMiscellaniousPage)
{
	mIcon = QIcon(":/icons/preferences/miscellaneous.png");
	mUi->setupUi(this);

	connect(mUi->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));

	mUi->colorComboBox->addItems(QColor::colorNames());

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

	SettingsManager::setValue("pathToImages", mUi->imagesPathEdit->text());
	SettingsManager::setValue("recentProjectsLimit", mUi->recentProjectsLimitSpinBox->value());
	SettingsManager::setValue("PaintOldEdgeMode", mUi->paintOldLineCheckBox->isChecked());
	SettingsManager::setValue("oldLineColor", mUi->colorComboBox->currentText());

	if (mLastIconsetPath != mUi->imagesPathEdit->text()) {
		emit iconsetChanged();
	}
}

void PreferencesMiscellaniousPage::restoreSettings()
{
	mUi->antialiasingCheckBox->setChecked(SettingsManager::value("Antialiasing").toBool());
	mUi->splashScreenCheckBox->setChecked(SettingsManager::value("Splashscreen").toBool());
	mUi->openGLCheckBox->setChecked(SettingsManager::value("OpenGL").toBool());

	mUi->paintOldLineCheckBox->setChecked(SettingsManager::value("PaintOldEdgeMode").toBool());

	QString curColor = SettingsManager::value("oldLineColor").toString();
	int curColorIndex = mUi->colorComboBox->findText(curColor);
	mUi->colorComboBox->setCurrentIndex(curColorIndex);

	mLastIconsetPath = SettingsManager::value("pathToImages").toString();
	mUi->imagesPathEdit->setText(mLastIconsetPath);
}
