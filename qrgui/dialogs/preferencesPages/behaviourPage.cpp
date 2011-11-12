#include "../../../qrkernel/settingsManager.h"
#include "behaviourPage.h"
#include "ui_behaviourPage.h"

PreferencesBehaviourPage::PreferencesBehaviourPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesBehaviourPage)
{
	mUi->setupUi(this);

	connect(mUi->autoSaveCheckBox, SIGNAL(clicked(bool)), this, SLOT(showAutoSaveBox(bool)));

	mUi->warningWindowBox->setChecked(SettingsManager::value("warningWindow", true).toBool());
	mUi->diagramCreateCheckBox->setChecked(SettingsManager::value("diagramCreateSuggestion", true).toBool());
	mUi->paletteTabCheckBox->setChecked(SettingsManager::value("PaletteTabSwitching", true).toBool());
	mUi->arrangeLinksCheckBox->setChecked(SettingsManager::value("arrangeLinks", true).toBool());
	mUi->autoSaveCheckBox->setChecked(SettingsManager::value("autoSave", true).toBool());
	mUi->autoSaveSpinBox->setValue(SettingsManager::value("autoSaveInterval", 60 * 10).toInt());
	mUi->gestureDelaySpinBox->setValue(SettingsManager::value("gestureDelay", 1000).toInt());

	showAutoSaveBox(mUi->autoSaveCheckBox->isChecked());
}

PreferencesBehaviourPage::~PreferencesBehaviourPage()
{
	delete mUi;
}

void PreferencesBehaviourPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesBehaviourPage::save()
{
	SettingsManager::setValue("diagramCreateSuggestion", mUi->diagramCreateCheckBox->isChecked());
	SettingsManager::setValue("PaletteTabSwitching", mUi->paletteTabCheckBox->isChecked());
	SettingsManager::setValue("warningWindow", mUi->warningWindowBox->isChecked());
	SettingsManager::setValue("arrangeLinks", mUi->arrangeLinksCheckBox->isChecked());
	SettingsManager::setValue("autoSave", mUi->autoSaveCheckBox->isChecked());
	SettingsManager::setValue("autoSaveInterval", mUi->autoSaveSpinBox->value());
	SettingsManager::setValue("gestureDelay", mUi->gestureDelaySpinBox->value());
}

void PreferencesBehaviourPage::showAutoSaveBox(bool show)
{
	mUi->autoSaveSpinBox->setVisible(show);
	mUi->autoSaveLabel->setVisible(show);
}
