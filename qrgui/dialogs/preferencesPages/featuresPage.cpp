#include "../../../qrkernel/settingsManager.h"

#include <QDebug>

#include "featuresPage.h"
#include "ui_featuresPage.h"

PreferencesFeaturesPage::PreferencesFeaturesPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesFeaturesPage)
{
	mUi->setupUi(this);

	qDebug() << SettingsManager::value("Gestures", true).toBool();
	qDebug() << SettingsManager::value("EmbeddedLinkers", true).toBool();
	qDebug() << SettingsManager::value("EmbeddedControls", true).toBool();

	mUi->gesturesCheckBox->setChecked(SettingsManager::value("Gestures", true).toBool());
	mUi->embeddedLinkersCheckBox->setChecked(SettingsManager::value("EmbeddedLinkers", true).toBool());
	mUi->embeddedControlsCheckBox->setChecked(SettingsManager::value("EmbeddedControls", false).toBool());
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

	qDebug() << SettingsManager::value("Gestures", true).toBool();
	qDebug() << SettingsManager::value("EmbeddedLinkers", true).toBool();
	qDebug() << SettingsManager::value("EmbeddedControls", true).toBool();
}
