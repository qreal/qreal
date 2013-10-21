#include "featuresPage.h"
#include "ui_featuresPage.h"

#include <qrkernel/settingsManager.h>

using namespace qReal;

PreferencesFeaturesPage::PreferencesFeaturesPage(QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesFeaturesPage)
{
	mIcon = QIcon(":/icons/preferences/features.png");
	mUi->setupUi(this);

	restoreSettings();
}

PreferencesFeaturesPage::~PreferencesFeaturesPage()
{
	delete mUi;
}

void PreferencesFeaturesPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesFeaturesPage::save()
{
	SettingsManager::setValue("Gestures", mUi->gesturesCheckBox->isChecked());
	SettingsManager::setValue("EmbeddedLinkers", mUi->embeddedLinkersCheckBox->isChecked());
	SettingsManager::setValue("EmbeddedControls", mUi->embeddedControlsCheckBox->isChecked());
}

void PreferencesFeaturesPage::restoreSettings()
{
	mUi->gesturesCheckBox->setChecked(SettingsManager::value("Gestures").toBool());
	mUi->embeddedLinkersCheckBox->setChecked(SettingsManager::value("EmbeddedLinkers").toBool());
	mUi->embeddedControlsCheckBox->setChecked(SettingsManager::value("EmbeddedControls").toBool());
}
