#include "editorPage.h"
#include "ui_editorPage.h"

#include <qrkernel/settingsManager.h>
#include <plugins/editorPluginInterface/elementImpl.h>

using namespace qReal;
using namespace enums::linkShape;

PreferencesEditorPage::PreferencesEditorPage(QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::PreferencesEditorPage)
	, mWidthGrid(SettingsManager::value("GridWidth").toInt())
	, mIndexGrid(SettingsManager::value("IndexGrid").toInt())
	, mMiniMapSize(SettingsManager::value("MiniMapSize").toInt())
	, mFontButtonWasPressed(false)
{
	mUi->setupUi(this);
	setObjectName("preferencesEditorPage");
	setWindowIcon(QIcon(":/preferencesDialog/images/editor.png"));

	connect(mUi->gridWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(widthGridSliderMoved(int)));
	connect(mUi->indexGridSlider, SIGNAL(valueChanged(int)), this, SLOT(indexGridSliderMoved(int)));
	connect(mUi->dragAreaSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(dragAreaSliderMoved(int)));
	connect(mUi->fontCheckBox, SIGNAL(toggled(bool)), this, SLOT(manualFontCheckBoxChecked(bool)));
	connect(mUi->fontSelectionButton, SIGNAL(clicked()),this, SLOT(fontSelectionButtonClicked()));
	connect(mUi->paletteComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(paletteComboBoxClicked(int)));
	connect(mUi->miniMapSize, &QSlider::sliderMoved, this, &PreferencesEditorPage::miniMapSizeSliderMoved);
	
	mUi->miniMapSize->setMinimum(50);
	mUi->miniMapSize->setMaximum(250);

	mUi->gridWidthSlider->setValue(mWidthGrid);
	mUi->indexGridSlider->setValue(mIndexGrid);
	mUi->miniMapSize->setValue(mMiniMapSize);

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
}

void PreferencesEditorPage::indexGridSliderMoved(int value)
{
	SettingsManager::setValue("IndexGrid", value);
}

void PreferencesEditorPage::miniMapSizeSliderMoved(int value)
{
	SettingsManager::setValue("MiniMapSize", value);
}

void PreferencesEditorPage::dragAreaSliderMoved(int value)
{
	SettingsManager::setValue("DragArea", value);
}

void PreferencesEditorPage::save()
{
	SettingsManager::setValue("EmbeddedLinkerIndent", mUi->embeddedLinkerIndentSlider->value());
	SettingsManager::setValue("EmbeddedLinkerSize", mUi->embeddedLinkerSizeSlider->value());
	SettingsManager::setValue("LineType", mUi->lineMode->currentIndex());
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

	mWidthGrid = mUi->gridWidthSlider->value();
	mIndexGrid = mUi->indexGridSlider->value();
	mDragArea = mUi->dragAreaSizeSlider->value();
	mMiniMapSize = mUi->miniMapSize->value();
	SettingsManager::setValue("GridWidth", mWidthGrid);
	SettingsManager::setValue("IndexGrid", mIndexGrid);
	SettingsManager::setValue("DragArea", mDragArea);
	SettingsManager::setValue("MiniMapSize", mMiniMapSize);

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

	LinkShape const type = static_cast<LinkShape>(SettingsManager::value("LineType").toInt());
	mUi->lineMode->setCurrentIndex(type);

	mUi->fontCheckBox->setChecked(SettingsManager::value("CustomFont").toBool());
	mUi->fontSelectionButton->setVisible(mUi->fontCheckBox->isChecked());
	mWasChecked = mUi->fontCheckBox->isChecked();

	mUi->paletteComboBox->setCurrentIndex(SettingsManager::value("PaletteRepresentation").toInt());
	paletteComboBoxClicked(mUi->paletteComboBox->currentIndex());
	mUi->paletteSpinBox->setValue(SettingsManager::value("PaletteIconsInARowCount").toInt());
	mFont = SettingsManager::value("CurrentFont").toString();
	mUi->miniMapSize->setValue(SettingsManager::value("MiniMapSize").toInt());
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
