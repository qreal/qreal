#include "../../kernel/settingsManager.h"
#include "editorPage.h"
#include "ui_editorPage.h"

PreferencesEditorPage::PreferencesEditorPage(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesEditorPage),
	mShowGridAction(showGridAction),
	mShowAlignmentAction(showAlignmentAction),
	mActivateGridAction(activateGridAction),
	mActivateAlignmentAction(activateAlignmentAction)
{
	mUi->setupUi(this);

	// changing grid size in QReal:Robots is forbidden
	connect(mUi->gridWidthSlider, SIGNAL(sliderMoved(int)), this, SLOT(widthGridSliderMoved(int)));
	connect(mUi->indexGridSlider, SIGNAL(sliderMoved(int)), this, SLOT(indexGridSliderMoved(int)));
	mUi->indexGridSlider->setVisible(false);
	mUi->label_20->setVisible(false);

	mUi->showGridCheckBox->setChecked(SettingsManager::value("ShowGrid", true).toBool());
	mUi->showAlignmentCheckBox->setChecked(SettingsManager::value("ShowAlignment", true).toBool());
	mUi->activateGridCheckBox->setChecked(SettingsManager::value("ActivateGrid", false).toBool());
	mUi->activateAlignmentCheckBox->setChecked(SettingsManager::value("ActivateAlignment", true).toBool());
	mUi->embeddedLinkerIndentSlider->setValue(SettingsManager::value("EmbeddedLinkerIndent", 8).toInt());
	mUi->embeddedLinkerSizeSlider->setValue(SettingsManager::value("EmbeddedLinkerSize", 6).toInt());
	mUi->zoomFactorSlider->setValue(SettingsManager::value("zoomFactor", 2).toInt());

	mWidthGrid = SettingsManager::value("GridWidth", 10).toInt();
	mIndexGrid = SettingsManager::value("IndexGrid", 30).toInt();
	mUi->gridWidthSlider->setValue(mWidthGrid);
	mUi->indexGridSlider->setValue(mIndexGrid);
}

PreferencesEditorPage::~PreferencesEditorPage()
{
	SettingsManager::setValue("GridWidth", mWidthGrid);
	SettingsManager::setValue("IndexGrid", mIndexGrid);

	delete mUi;
}

void PreferencesEditorPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesEditorPage::widthGridSliderMoved(int value)
{
	SettingsManager::setValue("GridWidth", value);
	emit gridChanged();
}

void PreferencesEditorPage::indexGridSliderMoved(int value)
{
	SettingsManager::setValue("IndexGrid", value);
	emit gridChanged();
}

void PreferencesEditorPage::save()
{
	SettingsManager::setValue("EmbeddedLinkerIndent", mUi->embeddedLinkerIndentSlider->value());
	SettingsManager::setValue("EmbeddedLinkerSize", mUi->embeddedLinkerSizeSlider->value());
	SettingsManager::setValue("zoomFactor", mUi->zoomFactorSlider->value());
	SettingsManager::setValue("ShowGrid", mUi->showGridCheckBox->isChecked());
	SettingsManager::setValue("ShowAlignment", mUi->showAlignmentCheckBox->isChecked());
	SettingsManager::setValue("ActivateGrid", mUi->activateGridCheckBox->isChecked());
	SettingsManager::setValue("ActivateAlignment", mUi->activateAlignmentCheckBox->isChecked());

	mWidthGrid = mUi->gridWidthSlider->value();
	mIndexGrid = mUi->indexGridSlider->value();
	SettingsManager::setValue("GridWidth", mWidthGrid);
	SettingsManager::setValue("IndexGrid", mIndexGrid);

	mShowGridAction->setChecked(mUi->showGridCheckBox->isChecked());
	mShowAlignmentAction->setChecked(mUi->showAlignmentCheckBox->isChecked());
	mActivateGridAction->setChecked(mUi->activateGridCheckBox->isChecked());
	mActivateAlignmentAction->setChecked(mUi->activateAlignmentCheckBox->isChecked());
}
