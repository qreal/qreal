#include "editorPage.h"
#include "ui_editorPage.h"

#include <qrkernel/settingsManager.h>

#include "mainwindow/mainWindow.h"

using namespace qReal;
using namespace enums::linkShape;

PreferencesEditorPage::PreferencesEditorPage(QAction * const showGridAction, QAction * const showAlignmentAction
		, QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::PreferencesEditorPage)
	, mWidthGrid(SettingsManager::value("GridWidth").toInt())
	, mIndexGrid(SettingsManager::value("IndexGrid").toInt())
	, mFontButtonWasPressed(false)
	, mShowGridAction(showGridAction)
	, mShowAlignmentAction(showAlignmentAction)
	, mActivateGridAction(activateGridAction)
	, mActivateAlignmentAction(activateAlignmentAction)
{
	mIcon = QIcon(":/icons/preferences/editor.png");
	mUi->setupUi(this);

	// changing grid size in QReal:Robots is forbidden
	connect(mUi->gridWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(widthGridSliderMoved(int)));
	connect(mUi->indexGridSlider, SIGNAL(valueChanged(int)), this, SLOT(indexGridSliderMoved(int)));
	connect(mUi->dragAreaSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(dragAreaSliderMoved(int)));
	connect(mUi->fontCheckBox, SIGNAL(toggled(bool)), this, SLOT(manualFontCheckBoxChecked(bool)));
	connect(mUi->fontSelectionButton, SIGNAL(clicked()),this, SLOT(fontSelectionButtonClicked()));
	connect(mUi->paletteComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(paletteComboBoxClicked(int)));

	connect(mShowGridAction, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(mShowAlignmentAction, SIGNAL(toggled(bool)), this, SLOT(showAlignment(bool)));
	connect(mActivateGridAction, SIGNAL(toggled(bool)), this, SLOT(activateGrid(bool)));
	connect(mActivateAlignmentAction, SIGNAL(toggled(bool)), this, SLOT(activateAlignment(bool)));

	// use customizer to blick it somehow
//	mUi->indexGridSlider->setVisible(false);
//	mUi->label_20->setVisible(false);

	mUi->gridWidthSlider->setValue(mWidthGrid);
	mUi->indexGridSlider->setValue(mIndexGrid);

	mDragArea = mUi->dragAreaSizeSlider->value();
	SettingsManager::setValue("DragArea", mDragArea);
	restoreSettings();
}

PreferencesEditorPage::~PreferencesEditorPage()
{
	SettingsManager::setValue("GridWidth", mWidthGrid);
	SettingsManager::setValue("IndexGrid", mIndexGrid);

	delete mUi;
}

void PreferencesEditorPage::manualFontCheckBoxChecked(bool state)
{
	mUi->fontSelectionButton->setVisible(state);
}

void PreferencesEditorPage::fontSelectionButtonClicked()
{
	mFontButtonWasPressed = true;

	QFontDialog fontDialog(this);
	fontDialog.setModal(true);
	QFont font;
	font.fromString(mFont);
	fontDialog.setCurrentFont(font);
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

void PreferencesEditorPage::dragAreaSliderMoved(int value)
{
	SettingsManager::setValue("DragArea", value);
}

void PreferencesEditorPage::save()
{
	SettingsManager::setValue("EmbeddedLinkerIndent", mUi->embeddedLinkerIndentSlider->value());
	SettingsManager::setValue("EmbeddedLinkerSize", mUi->embeddedLinkerSizeSlider->value());
	SettingsManager::setValue("LineType", mUi->lineMode->currentIndex() - 1);
	SettingsManager::setValue("LoopEdgeBoundsIndent", mUi->loopEdgeBoundsIndent->value());
	SettingsManager::setValue("ShowGrid", mUi->showGridCheckBox->isChecked());
	SettingsManager::setValue("ShowAlignment", mUi->showAlignmentCheckBox->isChecked());
	SettingsManager::setValue("ActivateGrid", mUi->activateGridCheckBox->isChecked());
	SettingsManager::setValue("ActivateAlignment", mUi->activateAlignmentCheckBox->isChecked());
	SettingsManager::setValue("CustomFont", mUi->fontCheckBox->isChecked());
	SettingsManager::setValue("PaletteRepresentation", mUi->paletteComboBox->currentIndex());
	SettingsManager::setValue("PaletteIconsInARowCount", mUi->paletteSpinBox->value());
	SettingsManager::setValue("MoveLabels", mUi->enableMoveLabelsCheckBox->isChecked());
	SettingsManager::setValue("ResizeLabels", mUi->enableResizeLabelsCheckBox->isChecked());
	SettingsManager::setValue("LabelsDistance", mUi->labelDistanceSlider->value());
	SettingsManager::setValue("manualFontCheckBoxChecked", mUi->fontCheckBox->isChecked());

	emit paletteRepresentationChanged();

	mWidthGrid = mUi->gridWidthSlider->value();
	mIndexGrid = mUi->indexGridSlider->value();
	mDragArea = mUi->dragAreaSizeSlider->value();
	SettingsManager::setValue("GridWidth", mWidthGrid);
	SettingsManager::setValue("IndexGrid", mIndexGrid);
	SettingsManager::setValue("DragArea", mDragArea);

	mShowGridAction->setChecked(mUi->showGridCheckBox->isChecked());
	mShowAlignmentAction->setChecked(mUi->showAlignmentCheckBox->isChecked());
	mActivateGridAction->setChecked(mUi->activateGridCheckBox->isChecked());
	mActivateAlignmentAction->setChecked(mUi->activateAlignmentCheckBox->isChecked());

	if (mWasChecked != mUi->fontCheckBox->isChecked() || mOldFont != mFont) {
		if (mFontButtonWasPressed) {
			SettingsManager::setValue("CurrentFont", mFont);
		}

		setRestartFlag();
		mFontButtonWasPressed = false;
	}
}

void PreferencesEditorPage::restoreSettings()
{
	mUi->showGridCheckBox->setChecked(SettingsManager::value("ShowGrid").toBool());
	mUi->showAlignmentCheckBox->setChecked(SettingsManager::value("ShowAlignment").toBool());
	mUi->activateGridCheckBox->setChecked(SettingsManager::value("ActivateGrid").toBool());
	mUi->activateAlignmentCheckBox->setChecked(SettingsManager::value("ActivateAlignment").toBool());
	mUi->embeddedLinkerIndentSlider->setValue(SettingsManager::value("EmbeddedLinkerIndent").toInt());
	mUi->embeddedLinkerSizeSlider->setValue(SettingsManager::value("EmbeddedLinkerSize").toInt());
	mUi->loopEdgeBoundsIndent->setValue(SettingsManager::value("LoopEdgeBoundsIndent").toInt());

	mUi->enableMoveLabelsCheckBox->setChecked(SettingsManager::value("MoveLabels").toBool());
	mUi->enableResizeLabelsCheckBox->setChecked(SettingsManager::value("ResizeLabels").toBool());
	mUi->labelDistanceSlider->setValue(SettingsManager::value("LabelsDistance").toInt());

	LinkShape const type = static_cast<LinkShape>(SettingsManager::value("LineType", unset).toInt());
	mUi->lineMode->setCurrentIndex(type + 1);

	mUi->fontCheckBox->setChecked(SettingsManager::value("CustomFont").toBool());
	mUi->fontSelectionButton->setVisible(mUi->fontCheckBox->isChecked());
	mWasChecked = mUi->fontCheckBox->isChecked();

	mUi->paletteComboBox->setCurrentIndex(SettingsManager::value("PaletteRepresentation").toInt());
	paletteComboBoxClicked(mUi->paletteComboBox->currentIndex());
	mUi->paletteSpinBox->setValue(SettingsManager::value("PaletteIconsInARowCount").toInt());
	mFont = SettingsManager::value("CurrentFont").toString();
	mOldFont = mFont;
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
	mUi->showGridCheckBox->setChecked(show);
}

void PreferencesEditorPage::showAlignment(bool show)
{
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
