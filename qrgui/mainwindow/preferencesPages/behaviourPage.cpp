#include "preferencesPages/behaviourPage.h"
#include "ui_preferencesbehaviourpage.h"

#include <QSettings>

PreferencesBehaviourPage::PreferencesBehaviourPage(QWidget *parent) :
	PreferencesPage(parent),
	ui(new Ui::preferencesBehaviourPage)
{
	ui->setupUi(this);

	QSettings settings("SPbSU", "QReal");
	ui->saveExitCheckBox->setChecked(settings.value("SaveExitSuggestion", true).toBool());
	ui->warningWindowBox->setChecked(settings.value("warningWindow", true).toBool());
	ui->chooseDiagramsToSaveCheckBox->setChecked(settings.value("ChooseDiagramsToSave", true).toBool());
	ui->diagramCreateCheckBox->setChecked(settings.value("DiagramCreateSuggestion", true).toBool());
	ui->paletteTabCheckBox->setChecked(settings.value("PaletteTabSwitching", true).toBool());
}

PreferencesBehaviourPage::~PreferencesBehaviourPage()
{
	delete ui;
}

void PreferencesBehaviourPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesBehaviourPage::save()
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("ChooseDiagramsToSave", ui->chooseDiagramsToSaveCheckBox->isChecked());
	settings.setValue("DiagramCreateSuggestion", ui->diagramCreateCheckBox->isChecked());
	settings.setValue("PaletteTabSwitching", ui->paletteTabCheckBox->isChecked());
	settings.setValue("SaveExitSuggestion", ui->saveExitCheckBox->isChecked());
	settings.setValue("warningWindow", ui->warningWindowBox->isChecked());
}
