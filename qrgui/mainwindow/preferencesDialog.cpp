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

	connect(mDebuggerUi->browseDebPathButton, SIGNAL(clicked()), this, SLOT(changeDebuggerPath()));
	connect(mDebuggerUi->builderPathButton, SIGNAL(clicked()), this, SLOT(changeBuilderPath()));
	connect(mDebuggerUi->workDirPushButton, SIGNAL(clicked()), this, SLOT(changeWorkingDirectory()));

	connect(ui->listWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(chooseTab(const QModelIndex &)));


	int currentTab = SettingsManager::value("currentPreferencesTab", 0).toInt();
	ui->listWidget->setCurrentRow(currentTab);
	chooseTab(ui->listWidget->currentIndex());
}

void PreferencesDialog::widthGridSliderMoved(int value)
{
	SettingsManager::setValue("GridWidth", value);
	emit gridChanged();
}

void PreferencesDialog::indexGridSliderMoved(int value)
{
	SettingsManager::setValue("IndexGrid", value);
	emit gridChanged();
}

PreferencesDialog::~PreferencesDialog()
{
	SettingsManager::setValue("currentPreferencesTab", ui->listWidget->currentRow());


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
	mEditorUi->embeddedLinkerIndentSlider->setValue(SettingsManager::value("EmbeddedLinkerIndent", 8).toInt());
	mEditorUi->embeddedLinkerSizeSlider->setValue(SettingsManager::value("EmbeddedLinkerSize", 6).toInt());
	mEditorUi->gridWidthSlider->setValue(SettingsManager::value("GridWidth", 10).toInt());
	mEditorUi->indexGridSlider->setValue(SettingsManager::value("IndexGrid", 30).toInt());
	mEditorUi->zoomFactorSlider->setValue(SettingsManager::value("zoomFactor", 2).toInt());
	mWithGrid = mEditorUi->gridWidthSlider->value();
	mIndexGrid = mEditorUi->indexGridSlider->value();

	mBehaviourUi->chooseDiagramsToSaveCheckBox->setChecked(SettingsManager::value("ChooseDiagramsToSave", true).toBool());
	mBehaviourUi->diagramCreateCheckBox->setChecked(SettingsManager::value("DiagramCreateSuggestion", true).toBool());
	mBehaviourUi->paletteTabCheckBox->setChecked(SettingsManager::value("PaletteTabSwitching", true).toBool());
	mMiscellaniousUi->chaoticEditionCheckBox->setChecked(SettingsManager::value("ChaoticEdition", false).toBool());
	mBehaviourUi->saveExitCheckBox->setChecked(SettingsManager::value("SaveExitSuggestion", true).toBool());
	mEditorUi->showGridCheckBox->setChecked(SettingsManager::value("ShowGrid", true).toBool());
	mEditorUi->showAlignmentCheckBox->setChecked(SettingsManager::value("ShowAlignment", true).toBool());
	mEditorUi->activateGridCheckBox->setChecked(SettingsManager::value("ActivateGrid", false).toBool());
	mEditorUi->activateAlignmentCheckBox->setChecked(SettingsManager::value("ActivateAlignment", true).toBool());
	mMiscellaniousUi->antialiasingCheckBox->setChecked(SettingsManager::value("Antialiasing", true).toBool());
	mMiscellaniousUi->splashScreenCheckBox->setChecked(SettingsManager::value("Splashscreen", true).toBool());
	mMiscellaniousUi->openGLCheckBox->setChecked(SettingsManager::value("OpenGL", true).toBool());
	mMiscellaniousUi->squareLineModeCheckBox->setChecked(SettingsManager::value("SquareLine", true).toBool());

	mBehaviourUi->warningWindowBox->setChecked(SettingsManager::value("warningWindow", true).toBool());

	mCompilerUi->windowsButton->setChecked(SettingsManager::value("windowsButton", false).toBool());
	mCompilerUi->linuxButton->setChecked(SettingsManager::value("linuxButton", false).toBool());
	mCompilerUi->otherButton->setChecked(SettingsManager::value("otherButton", false).toBool());

	mCompilerUi->pathToQmake->setText(SettingsManager::value("pathToQmake", "").toString());
	mCompilerUi->pathToMake->setText(SettingsManager::value("pathToMake", "").toString());
	mCompilerUi->pluginExtension->setText(SettingsManager::value("pluginExtension", "").toString());
	mCompilerUi->prefix->setText(SettingsManager::value("prefix", "").toString());

	mDebuggerUi->timeoutLineEdit->setText(SettingsManager::value("debuggerTimeout", 750).toString());
	mDebuggerUi->colorComboBox->addItems(QColor::colorNames());
	QString curColor = SettingsManager::value("debugColor", "red").toString();
	int curColorIndex = mDebuggerUi->colorComboBox->findText(curColor);
	mDebuggerUi->colorComboBox->setCurrentIndex(curColorIndex);
	SettingsManager::value("debugColor", mDebuggerUi->colorComboBox->currentText());
	mDebuggerUi->debuggerPathLineEdit->setText(SettingsManager::value("debuggerPath", "gdb.exe").toString());
	mDebuggerUi->builderPathLineEdit->setText(SettingsManager::value("builderPath", "gcc.exe").toString());
	mDebuggerUi->codeFileNameLineEdit->setText(SettingsManager::value("codeFileName", "code.c").toString());
	mDebuggerUi->buildedFileNameLineEdit->setText(SettingsManager::value("buildedFileName", "builded.exe").toString());
	mDebuggerUi->workDirLineEdit->setText(SettingsManager::value("debugWorkingDirectory", "").toString());
}

void PreferencesDialog::applyChanges()
{
	SettingsManager::setValue("EmbeddedLinkerIndent", mEditorUi->embeddedLinkerIndentSlider->value());
	SettingsManager::setValue("EmbeddedLinkerSize", mEditorUi->embeddedLinkerSizeSlider->value());
	SettingsManager::setValue("GridWidth", mEditorUi->gridWidthSlider->value());
	SettingsManager::setValue("IndexGrid", mEditorUi->indexGridSlider->value());
	SettingsManager::setValue("zoomFactor", mEditorUi->zoomFactorSlider->value());
	mWithGrid = mEditorUi->gridWidthSlider->value();
	mIndexGrid = mEditorUi->indexGridSlider->value();

	SettingsManager::setValue("ChooseDiagramsToSave", mBehaviourUi->chooseDiagramsToSaveCheckBox->isChecked());
	SettingsManager::setValue("DiagramCreateSuggestion", mBehaviourUi->diagramCreateCheckBox->isChecked());
	SettingsManager::setValue("PaletteTabSwitching", mBehaviourUi->paletteTabCheckBox->isChecked());
	SettingsManager::setValue("ChaoticEdition", mMiscellaniousUi->chaoticEditionCheckBox->isChecked());
	SettingsManager::setValue("SaveExitSuggestion", mBehaviourUi->saveExitCheckBox->isChecked());
	SettingsManager::setValue("Splashscreen", mMiscellaniousUi->splashScreenCheckBox->isChecked());
	SettingsManager::setValue("ShowGrid", mEditorUi->showGridCheckBox->isChecked());
	SettingsManager::setValue("ShowAlignment", mEditorUi->showAlignmentCheckBox->isChecked());
	SettingsManager::setValue("ActivateGrid", mEditorUi->activateGridCheckBox->isChecked());
	SettingsManager::setValue("ActivateAlignment", mEditorUi->activateAlignmentCheckBox->isChecked());
	SettingsManager::setValue("Antialiasing", mMiscellaniousUi->antialiasingCheckBox->isChecked());
	SettingsManager::setValue("OpenGL", mMiscellaniousUi->openGLCheckBox->isChecked());
	SettingsManager::setValue("SquareLine", mMiscellaniousUi->squareLineModeCheckBox->isChecked());

	SettingsManager::setValue("warningWindow", mBehaviourUi->warningWindowBox->isChecked());

	SettingsManager::setValue("windowsButton", mCompilerUi->windowsButton->isChecked());
	SettingsManager::setValue("linuxButton", mCompilerUi->linuxButton->isChecked());
	SettingsManager::setValue("otherButton", mCompilerUi->otherButton->isChecked());

	SettingsManager::setValue("pathToQmake", mCompilerUi->pathToQmake->text());
	SettingsManager::setValue("pathToMake", mCompilerUi->pathToMake->text());
	SettingsManager::setValue("pluginExtension", mCompilerUi->pluginExtension->text());
	SettingsManager::setValue("prefix", mCompilerUi->prefix->text());

	SettingsManager::setValue("debuggerTimeout", mDebuggerUi->timeoutLineEdit->text());
	SettingsManager::setValue("debugColor", mDebuggerUi->colorComboBox->currentText());
	SettingsManager::setValue("debuggerPath", mDebuggerUi->debuggerPathLineEdit->text());
	SettingsManager::setValue("builderPath", mDebuggerUi->builderPathLineEdit->text());
	SettingsManager::setValue("codeFileName", mDebuggerUi->codeFileNameLineEdit->text());
	SettingsManager::setValue("buildedFileName", mDebuggerUi->buildedFileNameLineEdit->text());
	SettingsManager::setValue("debugWorkingDirectory", mDebuggerUi->workDirLineEdit->text());

	mShowGridAction->setChecked(mEditorUi->showGridCheckBox->isChecked());
	mShowAlignmentAction->setChecked(mEditorUi->showAlignmentCheckBox->isChecked());
	mActivateGridAction->setChecked(mEditorUi->activateGridCheckBox->isChecked());
	mActivateAlignmentAction->setChecked(mEditorUi->activateAlignmentCheckBox->isChecked());

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

	SettingsManager::setValue("GridWidth", mWithGrid);
	SettingsManager::setValue("IndexGrid", mIndexGrid);
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

void PreferencesDialog::changeDebuggerPath()
{
	QString path = QFileDialog::getOpenFileName(this, "Open File", QString(), "gdb.*");
	if (path != NULL) {
		mDebuggerUi->debuggerPathLineEdit->setText(path);
	}
}

void PreferencesDialog::changeBuilderPath()
{
	QString path = QFileDialog::getOpenFileName(this, "Open File", QString(), "gcc.*");
	if (path != NULL) {
		mDebuggerUi->builderPathLineEdit->setText(path);
	}
}

void PreferencesDialog::changeWorkingDirectory()
{
	QString path = QFileDialog::getExistingDirectory(this, "Open Directory");
	mDebuggerUi->workDirLineEdit->setText(path.replace("\\", "/"));
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
