#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include "preferencesPages/behaviourPage.h"
#include "preferencesPages/compilerPage.h"
#include "preferencesPages/debuggerPage.h"
#include "preferencesPages/editorPage.h"
#include "preferencesPages/miscellaniousPage.h"
#include "preferencesPages/robotSettingsPage.h"

#include <QFileDialog>

#include <QMessageBox>

PreferencesDialog::PreferencesDialog(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent)
	: QDialog(parent), ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);

	mBehaviourPage = new PreferencesBehaviourPage(ui->pageContentWigdet);
	mCompilerPage = new PreferencesCompilerPage(ui->pageContentWigdet);
	mDebuggerPage = new PreferencesDebuggerPage(ui->pageContentWigdet);
	mMiscellaniousPage = new PreferencesMiscellaniousPage(ui->pageContentWigdet);
	mRobotSettingsPage = new PreferencesRobotSettingsPage(ui->pageContentWigdet);
	mEditorPage = new PreferencesEditorPage(showGridAction,
		showAlignmentAction, activateGridAction, activateAlignmentAction, ui->pageContentWigdet);

	connect(ui->listWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(chooseTab(const QModelIndex &)));
	connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	connect(mEditorPage, SIGNAL(gridChanged()), this, SIGNAL(gridChanged()));
	connect(mMiscellaniousPage, SIGNAL(iconsetChanged()), this, SIGNAL(iconsetChanged()));

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
	mBehaviourPage->save();
	mCompilerPage->save();
	mEditorPage->save();
	mDebuggerPage->save();
	mMiscellaniousPage->save();
	mRobotSettingsPage->save();

	emit settingsApplied();
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);

		mBehaviourPage->changeEvent(e);
		mCompilerPage->changeEvent(e);
		mDebuggerPage->changeEvent(e);
		mEditorPage->changeEvent(e);
		mMiscellaniousPage->changeEvent(e);
		mRobotSettingsPage->changeEvent(e);
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

void PreferencesDialog::chooseTab(const QModelIndex &index)
{
	mBehaviourPage->hide();
	mCompilerPage->hide();
	mEditorPage->hide();
	mDebuggerPage->hide();
	mMiscellaniousPage->hide();
	mRobotSettingsPage->hide();

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

	case robotSettings:
		mRobotSettingsPage->show();
		break;
	}
}
