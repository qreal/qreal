#include "../../kernel/settingsManager.h"
#include "behaviourPage.h"
#include "ui_behaviourPage.h"

PreferencesBehaviourPage::PreferencesBehaviourPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesBehaviourPage)
{
	mUi->setupUi(this);

	mUi->saveExitCheckBox->setChecked(SettingsManager::value("SaveExitSuggestion", true).toBool());
	mUi->warningWindowBox->setChecked(SettingsManager::value("warningWindow", true).toBool());
	mUi->chooseDiagramsToSaveCheckBox->setChecked(SettingsManager::value("ChooseDiagramsToSave", true).toBool());
	mUi->diagramCreateCheckBox->setChecked(SettingsManager::value("DiagramCreateSuggestion", true).toBool());
	mUi->paletteTabCheckBox->setChecked(SettingsManager::value("PaletteTabSwitching", true).toBool());
	mUi->arrangeLinksCheckBox->setChecked(SettingsManager::value("arrangeLinks", true).toBool());
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
	SettingsManager::setValue("ChooseDiagramsToSave", mUi->chooseDiagramsToSaveCheckBox->isChecked());
	SettingsManager::setValue("DiagramCreateSuggestion", mUi->diagramCreateCheckBox->isChecked());
	SettingsManager::setValue("PaletteTabSwitching", mUi->paletteTabCheckBox->isChecked());
	SettingsManager::setValue("SaveExitSuggestion", mUi->saveExitCheckBox->isChecked());
	SettingsManager::setValue("warningWindow", mUi->warningWindowBox->isChecked());
	SettingsManager::setValue("arrangeLinks", mUi->arrangeLinksCheckBox->isChecked());
}
