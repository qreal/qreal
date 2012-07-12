#include "../../../qrkernel/settingsManager.h"
#include "editorPage.h"
#include "ui_editorPage.h"
#include <QMessageBox>
#include "../mainwindow/mainWindow.h"

PreferencesEditorPage::PreferencesEditorPage(QAction * const showGridAction, QAction * const showAlignmentAction
		, QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent)
		: PreferencesPage(parent)
		, mUi(new Ui::PreferencesEditorPage)
		, mFontWasChanged(false)
		, mFontButtonWasPressed(false)
		, mWidthGrid(SettingsManager::value("GridWidth").toInt())
		, mIndexGrid(SettingsManager::value("IndexGrid").toInt())
		, mShowGridAction(showGridAction)
		, mShowAlignmentAction(showAlignmentAction)
		, mActivateGridAction(activateGridAction)
		, mActivateAlignmentAction(activateAlignmentAction)
{
	mIcon = QIcon(":/icons/preferences/editor.png");
	mUi->setupUi(this);

	// changing grid size in QReal:Robots is forbidden
	connect(mUi->gridWidthSlider, SIGNAL(sliderMoved(int)), this, SLOT(widthGridSliderMoved(int)));
	connect(mUi->indexGridSlider, SIGNAL(sliderMoved(int)), this, SLOT(indexGridSliderMoved(int)));
	connect(mUi->fontCheckBox, SIGNAL(toggled(bool)), this, SLOT(manualFontCheckBoxChecked(bool)));
	connect(mUi->fontSelectionButton, SIGNAL(clicked()),this, SLOT(fontSelectionButtonClicked()));
	connect(mUi->paletteComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(paletteComboBoxClicked(int)));

	connect(mShowGridAction, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(mShowAlignmentAction, SIGNAL(toggled(bool)), this, SLOT(showAlignment(bool)));
	connect(mActivateGridAction, SIGNAL(toggled(bool)), this, SLOT(activateGrid(bool)));
	connect(mActivateAlignmentAction, SIGNAL(toggled(bool)), this, SLOT(activateAlignment(bool)));

	mUi->indexGridSlider->setVisible(false);
	mUi->label_20->setVisible(false);

	mUi->showGridCheckBox->setChecked(SettingsManager::value("ShowGrid").toBool());
	mUi->showAlignmentCheckBox->setChecked(SettingsManager::value("ShowAlignment").toBool());
	mUi->activateGridCheckBox->setChecked(SettingsManager::value("ActivateGrid").toBool());
	mUi->activateAlignmentCheckBox->setChecked(SettingsManager::value("ActivateAlignment").toBool());
	mUi->embeddedLinkerIndentSlider->setValue(SettingsManager::value("EmbeddedLinkerIndent", 8).toInt());
	mUi->embeddedLinkerSizeSlider->setValue(SettingsManager::value("EmbeddedLinkerSize").toInt());
	mUi->zoomFactorSlider->setValue(SettingsManager::value("zoomFactor", 2).toInt());

	mUi->gridWidthSlider->setValue(mWidthGrid);
	mUi->indexGridSlider->setValue(mIndexGrid);
	mUi->fontCheckBox->setChecked(SettingsManager::value("CustomFont", false).toBool());
	mUi->fontSelectionButton->setVisible(SettingsManager::value("CustomFont", false).toBool());

	mUi->paletteComboBox->setCurrentIndex(SettingsManager::value("PaletteRepresentation").toInt());
	paletteComboBoxClicked(mUi->paletteComboBox->currentIndex());
	mUi->paletteSpinBox->setValue(SettingsManager::value("PaletteIconsInARowCount").toInt());
	mFont = SettingsManager::value("CurrentFont", "").toString();
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
	if (!mFontWasChanged) {
		mFontWasChanged = !mFontWasChanged;
	}
	mFontButtonWasPressed = true;

	QFontDialog fontDialog(this);
	fontDialog.setModal(true);
	QFont f;
	f.fromString(mFont);
	fontDialog.setCurrentFont(f);
	fontDialog.exec();
	mFont = fontDialog.currentFont().toString();
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
	SettingsManager::setValue("PaletteRepresentation", mUi->paletteComboBox->currentIndex());
	SettingsManager::setValue("PaletteIconsInARowCount", mUi->paletteSpinBox->value());

	emit paletteRepresentationChanged();

	mWidthGrid = mUi->gridWidthSlider->value();
	mIndexGrid = mUi->indexGridSlider->value();
	SettingsManager::setValue("GridWidth", mWidthGrid);
	SettingsManager::setValue("IndexGrid", mIndexGrid);

	mShowGridAction->setChecked(mUi->showGridCheckBox->isChecked());
	mShowAlignmentAction->setChecked(mUi->showAlignmentCheckBox->isChecked());
	mActivateGridAction->setChecked(mUi->activateGridCheckBox->isChecked());
	mActivateAlignmentAction->setChecked(mUi->activateAlignmentCheckBox->isChecked());

	if (mFontWasChanged) {
		if (mFontButtonWasPressed) {
			SettingsManager::setValue("CurrentFont", mFont);
		}
		QMessageBox::information(NULL, tr("Information"), tr("You should restart QReal:Robots to apply changes"), tr("Ok"));
		mFontWasChanged = false;
		mFontButtonWasPressed = false;
	}
}
void PreferencesEditorPage::paletteComboBoxClicked(int index)
{
	mUi->paletteSpinBox->setEnabled((bool)index);
}

void PreferencesEditorPage::changePaletteParameters()
{
	mUi->paletteComboBox->setCurrentIndex(SettingsManager::value("PaletteRepresentation").toInt());
	mUi->paletteSpinBox->setValue(SettingsManager::value("PaletteIconsInARowCount").toInt());
}

void PreferencesEditorPage::showGrid(bool show)
{
//	SettingsManager::setValue("ShowGrid", show);
	mUi->showGridCheckBox->setChecked(show);
}

void PreferencesEditorPage::showAlignment(bool show)
{
//	SettingsManager::setValue("ShowAlignment", show);
	mUi->showAlignmentCheckBox->setChecked(show);
}

void PreferencesEditorPage::activateGrid(bool activate)
{
	mUi->activateGridCheckBox->setChecked(activate);
}

void PreferencesEditorPage::activateAlignment(bool activate)
{
	mUi->activateAlignmentCheckBox->setChecked(activate);
}
