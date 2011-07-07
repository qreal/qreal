#include "preferencesPages/editorPage.h"
#include "ui_editorPage.h"

#include <QSettings>

PreferencesEditorPage::PreferencesEditorPage(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent) :
	PreferencesPage(parent),
	ui(new Ui::PreferencesEditorPage),
	mShowGridAction(showGridAction),
	mShowAlignmentAction(showAlignmentAction),
	mActivateGridAction(activateGridAction),
	mActivateAlignmentAction(activateAlignmentAction)
{
	ui->setupUi(this);

	// changing grid size in QReal:Robots is forbidden
	connect(ui->gridWidthSlider, SIGNAL(sliderMoved(int)), this, SLOT(widthGridSliderMoved(int)));
	connect(ui->indexGridSlider, SIGNAL(sliderMoved(int)), this, SLOT(indexGridSliderMoved(int)));
	ui->indexGridSlider->setVisible(false);
	ui->label_20->setVisible(false);

	QSettings settings("SPbSU", "QReal");
	ui->showGridCheckBox->setChecked(settings.value("ShowGrid", true).toBool());
	ui->showAlignmentCheckBox->setChecked(settings.value("ShowAlignment", true).toBool());
	ui->activateGridCheckBox->setChecked(settings.value("ActivateGrid", false).toBool());
	ui->activateAlignmentCheckBox->setChecked(settings.value("ActivateAlignment", true).toBool());
	ui->embeddedLinkerIndentSlider->setValue(settings.value("EmbeddedLinkerIndent", 8).toInt());
	ui->embeddedLinkerSizeSlider->setValue(settings.value("EmbeddedLinkerSize", 6).toInt());
	ui->zoomFactorSlider->setValue(settings.value("zoomFactor", 2).toInt());

	mWidthGrid = settings.value("GridWidth", 10).toInt();
	mIndexGrid = settings.value("IndexGrid", 30).toInt();
	ui->gridWidthSlider->setValue(mWidthGrid);
	ui->indexGridSlider->setValue(mIndexGrid);
}

PreferencesEditorPage::~PreferencesEditorPage()
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("GridWidth", mWidthGrid);
	settings.setValue("IndexGrid", mIndexGrid);

	delete ui;
}

void PreferencesEditorPage::changeEvent(QEvent *e)
{
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesEditorPage::widthGridSliderMoved(int value)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("GridWidth", value);
	emit gridChanged();
}

void PreferencesEditorPage::indexGridSliderMoved(int value)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("IndexGrid", value);
	emit gridChanged();
}

void PreferencesEditorPage::save(){
	QSettings settings("SPbSU", "QReal");
	settings.setValue("EmbeddedLinkerIndent", ui->embeddedLinkerIndentSlider->value());
	settings.setValue("EmbeddedLinkerSize", ui->embeddedLinkerSizeSlider->value());
	settings.setValue("zoomFactor", ui->zoomFactorSlider->value());
	settings.setValue("ShowGrid", ui->showGridCheckBox->isChecked());
	settings.setValue("ShowAlignment", ui->showAlignmentCheckBox->isChecked());
	settings.setValue("ActivateGrid", ui->activateGridCheckBox->isChecked());
	settings.setValue("ActivateAlignment", ui->activateAlignmentCheckBox->isChecked());

	mWidthGrid = ui->gridWidthSlider->value();
	mIndexGrid = ui->indexGridSlider->value();
	settings.setValue("GridWidth", mWidthGrid);
	settings.setValue("IndexGrid", mIndexGrid);

	mShowGridAction->setChecked(ui->showGridCheckBox->isChecked());
	mShowAlignmentAction->setChecked(ui->showAlignmentCheckBox->isChecked());
	mActivateGridAction->setChecked(ui->activateGridCheckBox->isChecked());
	mActivateAlignmentAction->setChecked(ui->activateAlignmentCheckBox->isChecked());
}
