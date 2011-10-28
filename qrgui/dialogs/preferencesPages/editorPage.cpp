#include "../../../qrkernel/settingsManager.h"
#include "editorPage.h"
#include "ui_editorPage.h"
#include <QMessageBox>

PreferencesEditorPage::PreferencesEditorPage(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent) :
	PreferencesPage(parent),
	mUi(new Ui::PreferencesEditorPage),
	mFontWasChanged(false),
	mFontButtonWasPressed(false),
	mWidthGrid(SettingsManager::value("GridWidth", 10).toInt()),
	mIndexGrid(SettingsManager::value("IndexGrid", 50).toInt()),
	mShowGridAction(showGridAction),
	mShowAlignmentAction(showAlignmentAction),
	mActivateGridAction(activateGridAction),
	mActivateAlignmentAction(activateAlignmentAction)
{
	mUi->setupUi(this);

	// changing grid size in QReal:Robots is forbidden
	connect(mUi->gridWidthSlider, SIGNAL(sliderMoved(int)), this, SLOT(widthGridSliderMoved(int)));
	connect(mUi->indexGridSlider, SIGNAL(sliderMoved(int)), this, SLOT(indexGridSliderMoved(int)));
	connect(mUi->fontCheckBox, SIGNAL(toggled(bool)), this, SLOT(manualFontCheckBoxChecked(bool)));
	connect(mUi->fontSelectionButton, SIGNAL(clicked()),this, SLOT(fontSelectionButtonClicked()));
	mUi->indexGridSlider->setVisible(false);
	mUi->label_20->setVisible(false);

	mUi->showGridCheckBox->setChecked(SettingsManager::value("ShowGrid", true).toBool());
	mUi->showAlignmentCheckBox->setChecked(SettingsManager::value("ShowAlignment", true).toBool());
	mUi->activateGridCheckBox->setChecked(SettingsManager::value("ActivateGrid", true).toBool());
	mUi->activateAlignmentCheckBox->setChecked(SettingsManager::value("ActivateAlignment", true).toBool());
	mUi->embeddedLinkerIndentSlider->setValue(SettingsManager::value("EmbeddedLinkerIndent", 8).toInt());
	mUi->embeddedLinkerSizeSlider->setValue(SettingsManager::value("EmbeddedLinkerSize", 6).toInt());
	mUi->zoomFactorSlider->setValue(SettingsManager::value("zoomFactor", 2).toInt());

	mUi->gridWidthSlider->setValue(mWidthGrid);
	mUi->indexGridSlider->setValue(mIndexGrid);
	mUi->fontCheckBox->setChecked(SettingsManager::value("CustomFont", false).toBool());
	mUi->fontSelectionButton->setVisible(SettingsManager::value("CustomFont", false).toBool());
}

PreferencesEditorPage::~PreferencesEditorPage()
{
	SettingsManager::setValue("GridWidth", mWidthGrid);
	SettingsManager::setValue("IndexGrid", mIndexGrid);

	delete mUi;
}

void PreferencesEditorPage::manualFontCheckBoxChecked(bool state) {
	SettingsManager::setValue("manualFontCheckBoxChecked", state);

	mFontWasChanged = !mFontWasChanged;
	mUi->fontSelectionButton->setVisible(state);
}

void PreferencesEditorPage::fontSelectionButtonClicked() {
	if (!mFontWasChanged)
		mFontWasChanged = !mFontWasChanged;
	mFontButtonWasPressed = true;
	mFontDialog = new QFontDialog();
	mFontDialog->setModal(true);
	mFontDialog->exec();
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
	SettingsManager::setValue("CustomFont", mUi->fontCheckBox->isChecked());


	mWidthGrid = mUi->gridWidthSlider->value();
	mIndexGrid = mUi->indexGridSlider->value();
	SettingsManager::setValue("GridWidth", mWidthGrid);
	SettingsManager::setValue("IndexGrid", mIndexGrid);

	mShowGridAction->setChecked(mUi->showGridCheckBox->isChecked());
	mShowAlignmentAction->setChecked(mUi->showAlignmentCheckBox->isChecked());
	mActivateGridAction->setChecked(mUi->activateGridCheckBox->isChecked());
	mActivateAlignmentAction->setChecked(mUi->activateAlignmentCheckBox->isChecked());

	if (mFontWasChanged) {
		if (mFontButtonWasPressed)
			SettingsManager::setValue("CurrentFont", mFontDialog->currentFont().toString());
		QMessageBox::information(NULL, "Information", "You should restart QReal:Robots to apply changes", "ok");
		mFontWasChanged = false;
		mFontButtonWasPressed = false;
	}
}
