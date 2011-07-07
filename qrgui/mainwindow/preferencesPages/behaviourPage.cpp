#include "../../kernel/settingsManager.h"
#include "preferencesPages/behaviourPage.h"
#include "ui_behaviourPage.h"

PreferencesBehaviourPage::PreferencesBehaviourPage(QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesBehaviourPage)
{
	mUi->setupUi(this);

	mUi->saveExitCheckBox->setChecked(SettingsManager::instance()->value("SaveExitSuggestion", true).toBool());
	mUi->warningWindowBox->setChecked(SettingsManager::instance()->value("warningWindow", true).toBool());
	mUi->chooseDiagramsToSaveCheckBox->setChecked(SettingsManager::instance()->value("ChooseDiagramsToSave", true).toBool());
	mUi->diagramCreateCheckBox->setChecked(SettingsManager::instance()->value("DiagramCreateSuggestion", true).toBool());
	mUi->paletteTabCheckBox->setChecked(SettingsManager::instance()->value("PaletteTabSwitching", true).toBool());
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
	SettingsManager::instance()->setValue("ChooseDiagramsToSave", mUi->chooseDiagramsToSaveCheckBox->isChecked());
	SettingsManager::instance()->setValue("DiagramCreateSuggestion", mUi->diagramCreateCheckBox->isChecked());
	SettingsManager::instance()->setValue("PaletteTabSwitching", mUi->paletteTabCheckBox->isChecked());
	SettingsManager::instance()->setValue("SaveExitSuggestion", mUi->saveExitCheckBox->isChecked());
	SettingsManager::instance()->setValue("warningWindow", mUi->warningWindowBox->isChecked());
}
