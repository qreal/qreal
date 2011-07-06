#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include "ui_preferencesBehaviourForm.h"
#include "ui_preferencesCompilerForm.h"
#include "ui_preferencesDebuggerForm.h"
#include "ui_preferencesEditorForm.h"
#include "ui_preferencesMiscellaniousForm.h"

#include <QSettings>
#include <QFileDialog>

#include <QMessageBox>

PreferencesDialog::PreferencesDialog(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent)
	: QDialog(parent), ui(new Ui::PreferencesDialog), mShowGridAction(showGridAction), mShowAlignmentAction(showAlignmentAction)
	, mActivateGridAction(activateGridAction), mActivateAlignmentAction(activateAlignmentAction)
{
	ui->setupUi(this);
	initTabs();
	initPreferences();

	connect(mCompilerUi->linuxButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(mCompilerUi->windowsButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(mCompilerUi->otherButton, SIGNAL(clicked()), this, SLOT(changeSystem()));
	connect(mEditorUi->gridWidthSlider, SIGNAL(sliderMoved(int)), this, SLOT(widthGridSliderMoved(int)));
	connect(mEditorUi->indexGridSlider, SIGNAL(sliderMoved(int)), this, SLOT(indexGridSliderMoved(int)));
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
	connect(mMiscellaniousUi->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));

	connect(ui->listWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(chooseTab(const QModelIndex &)));

	// changing grid size in QReal:Robots is forbidden
	mEditorUi->indexGridSlider->setVisible(false);
	mEditorUi->label_20->setVisible(false);

	mLastIconsetPath = mMiscellaniousUi->imagesPathEdit->text();

	QSettings settings("SPbSU", "QReal");
	int currentTab = settings.value("currentPreferencesTab", 0).toInt();
	ui->listWidget->setCurrentRow(currentTab);
	chooseTab(ui->listWidget->currentIndex());
}

void PreferencesDialog::widthGridSliderMoved(int value)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("GridWidth", value);
	emit gridChanged();
}

void PreferencesDialog::indexGridSliderMoved(int value)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("IndexGrid", value);
	emit gridChanged();
}

PreferencesDialog::~PreferencesDialog()
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("currentPreferencesTab", ui->listWidget->currentRow());

	delete ui;

	delete mBehaviourUi;
	delete mCompilerUi;
	delete mDebuggerUi;
	delete mEditorUi;
	delete mMiscellaniousUi;

	delete mBehaviourPage;
	delete mCompilerPage;
	delete mDebuggerPage;
	delete mEditorPage;
	delete mMiscellaniousPage;
}


void PreferencesDialog::initTabs()
{
	mBehaviourPage = new QWidget(ui->pageContentWigdet);
	mBehaviourUi = new Ui::BehaviourForm;
	mBehaviourUi->setupUi(mBehaviourPage);
	mBehaviourPage->hide();

	mCompilerPage = new QWidget(ui->pageContentWigdet);
	mCompilerUi = new Ui::CompilerForm;
	mCompilerUi->setupUi(mCompilerPage);
	mCompilerPage->hide();

	mDebuggerPage = new QWidget(ui->pageContentWigdet);
	mDebuggerUi = new Ui::DebuggerForm;
	mDebuggerUi->setupUi(mDebuggerPage);
	mDebuggerPage->hide();

	mEditorPage = new QWidget(ui->pageContentWigdet);
	mEditorUi = new Ui::EditorForm;
	mEditorUi->setupUi(mEditorPage);
	mEditorPage->hide();

	mMiscellaniousPage = new QWidget(ui->pageContentWigdet);
	mMiscellaniousUi = new Ui::MiscellaniousForm;
	mMiscellaniousUi->setupUi(mMiscellaniousPage);
	mMiscellaniousPage->hide();
}

void PreferencesDialog::initPreferences()
{
	QSettings settings("SPbSU", "QReal");
	mMiscellaniousUi->imagesPathEdit->setText(settings.value("pathToImages", QDir::currentPath() + "/images/iconset1").toString());

	mEditorUi->embeddedLinkerIndentSlider->setValue(settings.value("EmbeddedLinkerIndent", 8).toInt());
	mEditorUi->embeddedLinkerSizeSlider->setValue(settings.value("EmbeddedLinkerSize", 6).toInt());
	mEditorUi->gridWidthSlider->setValue(settings.value("GridWidth", 10).toInt());
	mEditorUi->indexGridSlider->setValue(settings.value("IndexGrid", 30).toInt());
	mEditorUi->zoomFactorSlider->setValue(settings.value("zoomFactor", 2).toInt());
	mWithGrid = mEditorUi->gridWidthSlider->value();
	mIndexGrid = mEditorUi->indexGridSlider->value();

	mBehaviourUi->chooseDiagramsToSaveCheckBox->setChecked(settings.value("ChooseDiagramsToSave", true).toBool());
	mBehaviourUi->diagramCreateCheckBox->setChecked(settings.value("DiagramCreateSuggestion", true).toBool());
	mBehaviourUi->paletteTabCheckBox->setChecked(settings.value("PaletteTabSwitching", true).toBool());
	mMiscellaniousUi->chaoticEditionCheckBox->setChecked(settings.value("ChaoticEdition", false).toBool());
	mBehaviourUi->saveExitCheckBox->setChecked(settings.value("SaveExitSuggestion", true).toBool());
	mEditorUi->showGridCheckBox->setChecked(settings.value("ShowGrid", true).toBool());
	mEditorUi->showAlignmentCheckBox->setChecked(settings.value("ShowAlignment", true).toBool());
	mEditorUi->activateGridCheckBox->setChecked(settings.value("ActivateGrid", false).toBool());
	mEditorUi->activateAlignmentCheckBox->setChecked(settings.value("ActivateAlignment", true).toBool());
	mMiscellaniousUi->antialiasingCheckBox->setChecked(settings.value("Antialiasing", true).toBool());
	mMiscellaniousUi->splashScreenCheckBox->setChecked(settings.value("Splashscreen", true).toBool());
	mMiscellaniousUi->openGLCheckBox->setChecked(settings.value("OpenGL", true).toBool());
	mMiscellaniousUi->squareLineModeCheckBox->setChecked(settings.value("SquareLine", false).toBool());

	mBehaviourUi->warningWindowBox->setChecked(settings.value("warningWindow", true).toBool());

	mCompilerUi->windowsButton->setChecked(settings.value("windowsButton", false).toBool());
	mCompilerUi->linuxButton->setChecked(settings.value("linuxButton", false).toBool());
	mCompilerUi->otherButton->setChecked(settings.value("otherButton", false).toBool());

	mCompilerUi->pathToQmake->setText(settings.value("pathToQmake", "").toString());
	mCompilerUi->pathToMake->setText(settings.value("pathToMake", "").toString());
	mCompilerUi->pluginExtension->setText(settings.value("pluginExtension", "").toString());
	mCompilerUi->prefix->setText(settings.value("prefix", "").toString());

	mDebuggerUi->timeoutLineEdit->setText(settings.value("debuggerTimeout", 750).toString());
	mDebuggerUi->colorComboBox->addItems(QColor::colorNames());
	QString curColor = settings.value("debugColor", "red").toString();
	int curColorIndex = mDebuggerUi->colorComboBox->findText(curColor);
	mDebuggerUi->colorComboBox->setCurrentIndex(curColorIndex);
	settings.value("debugColor", mDebuggerUi->colorComboBox->currentText());
}

void PreferencesDialog::applyChanges()
{
	QSettings settings("SPbSU", "QReal");

	settings.setValue("pathToImages", mMiscellaniousUi->imagesPathEdit->text());

	settings.setValue("EmbeddedLinkerIndent", mEditorUi->embeddedLinkerIndentSlider->value());
	settings.setValue("EmbeddedLinkerSize", mEditorUi->embeddedLinkerSizeSlider->value());
	settings.setValue("GridWidth", mEditorUi->gridWidthSlider->value());
	settings.setValue("IndexGrid", mEditorUi->indexGridSlider->value());
	settings.setValue("zoomFactor", mEditorUi->zoomFactorSlider->value());
	mWithGrid = mEditorUi->gridWidthSlider->value();
	mIndexGrid = mEditorUi->indexGridSlider->value();

	settings.setValue("ChooseDiagramsToSave", mBehaviourUi->chooseDiagramsToSaveCheckBox->isChecked());
	settings.setValue("DiagramCreateSuggestion", mBehaviourUi->diagramCreateCheckBox->isChecked());
	settings.setValue("PaletteTabSwitching", mBehaviourUi->paletteTabCheckBox->isChecked());
	settings.setValue("ChaoticEdition", mMiscellaniousUi->chaoticEditionCheckBox->isChecked());
	settings.setValue("SaveExitSuggestion", mBehaviourUi->saveExitCheckBox->isChecked());
	settings.setValue("Splashscreen", mMiscellaniousUi->splashScreenCheckBox->isChecked());
	settings.setValue("ShowGrid", mEditorUi->showGridCheckBox->isChecked());
	settings.setValue("ShowAlignment", mEditorUi->showAlignmentCheckBox->isChecked());
	settings.setValue("ActivateGrid", mEditorUi->activateGridCheckBox->isChecked());
	settings.setValue("ActivateAlignment", mEditorUi->activateAlignmentCheckBox->isChecked());
	settings.setValue("Antialiasing", mMiscellaniousUi->antialiasingCheckBox->isChecked());
	settings.setValue("OpenGL", mMiscellaniousUi->openGLCheckBox->isChecked());
	settings.setValue("SquareLine", mMiscellaniousUi->squareLineModeCheckBox->isChecked());

	settings.setValue("warningWindow", mBehaviourUi->warningWindowBox->isChecked());

	settings.setValue("windowsButton", mCompilerUi->windowsButton->isChecked());
	settings.setValue("linuxButton", mCompilerUi->linuxButton->isChecked());
	settings.setValue("otherButton", mCompilerUi->otherButton->isChecked());

	settings.setValue("pathToQmake", mCompilerUi->pathToQmake->text());
	settings.setValue("pathToMake", mCompilerUi->pathToMake->text());
	settings.setValue("pluginExtension", mCompilerUi->pluginExtension->text());
	settings.setValue("prefix", mCompilerUi->prefix->text());

	settings.setValue("debuggerTimeout", mDebuggerUi->timeoutLineEdit->text());
	settings.setValue("debugColor", mDebuggerUi->colorComboBox->currentText());

	mShowGridAction->setChecked(mEditorUi->showGridCheckBox->isChecked());
	mShowAlignmentAction->setChecked(mEditorUi->showAlignmentCheckBox->isChecked());
	mActivateGridAction->setChecked(mEditorUi->activateGridCheckBox->isChecked());
	mActivateAlignmentAction->setChecked(mEditorUi->activateAlignmentCheckBox->isChecked());

	if (mLastIconsetPath != mMiscellaniousUi->imagesPathEdit->text())
		emit iconsetChanged();

	emit settingsApplied();
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		mBehaviourUi->retranslateUi(mBehaviourPage);
		mCompilerUi->retranslateUi(mCompilerPage);
		mDebuggerUi->retranslateUi(mDebuggerPage);
		mEditorUi->retranslateUi(mEditorPage);
		mMiscellaniousUi->retranslateUi(mMiscellaniousPage);

		break;
	default:
		break;
	}
}

void PreferencesDialog::saveAndClose()
{
	applyChanges();
	close();
}

void PreferencesDialog::cancel()
{
	mEditorUi->gridWidthSlider->setValue(mWithGrid);
	mEditorUi->indexGridSlider->setValue(mIndexGrid);
	QSettings settings("SPbSU", "QReal");
	settings.setValue("GridWidth", mWithGrid);
	settings.setValue("IndexGrid", mIndexGrid);
	close();
}

void PreferencesDialog::changeSystem()
{
	if(mCompilerUi->linuxButton->isChecked())
		initCompilersSettings("qmake", "make", "so", "lib");
	if(mCompilerUi->windowsButton->isChecked())
		initCompilersSettings("qmake", "mingw32-make", "dll", "");
	if(mCompilerUi->otherButton->isChecked())
		mCompilerUi->compilerSettingsWidget->setEnabled(true);
}

void PreferencesDialog::initCompilersSettings(const QString &pathToQmake,
		const QString &pathToMake, const QString &pluginExtension, const QString &prefix)
{
	mCompilerUi->pathToQmake->setText(pathToQmake);
	mCompilerUi->pathToMake->setText(pathToMake);
	mCompilerUi->pluginExtension->setText(pluginExtension);
	mCompilerUi->prefix->setText(prefix);
	mCompilerUi->compilerSettingsWidget->setEnabled(false);
}

void PreferencesDialog::browseImagesPath()
{
	QString path = QFileDialog::getExistingDirectory(this, "Open Directory");
	mMiscellaniousUi->imagesPathEdit->setText(path.replace("\\", "/"));
}

void PreferencesDialog::chooseTab(const QModelIndex &index){
	mBehaviourPage->hide();
	mCompilerPage->hide();
	mEditorPage->hide();
	mDebuggerPage->hide();
	mMiscellaniousPage->hide();

	switch(static_cast<PageIndexes>(index.row())){
	case behaviour:
		mBehaviourPage->show();
		break;

	case compiler:
		mCompilerPage->show();
		break;

	case debugger:
		mDebuggerPage->show();
		break;

	case editor:
		mEditorPage->show();
		break;

	case miscellanious:
		mMiscellaniousPage->show();
		break;
	}
}
