#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include "ui_preferencesBehaviourForm.h"
#include "ui_preferencesCompilerForm.h"
#include "ui_preferencesDebuggerForm.h"
#include "ui_preferencesEditorForm.h"
#include "ui_preferencesMiscellaniousForm.h"

#include <QFileDialog>
#include <QMessageBox>

PreferencesDialog::PreferencesDialog(QAction * const showGridAction, QAction * const showAlignmentAction
									,QAction * const activateGridAction, QAction * const activateAlignmentAction
									 , QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::PreferencesDialog), mShowGridAction(showGridAction), mShowAlignmentAction(showAlignmentAction)
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

	int currentTab = SettingsManager::instance()->value("currentPreferencesTab", 0).toInt();
	ui->listWidget->setCurrentRow(currentTab);
	chooseTab(ui->listWidget->currentIndex());
}

void PreferencesDialog::widthGridSliderMoved(int value)
{
	SettingsManager::instance()->setValue("GridWidth", value);
	emit gridChanged();
}

void PreferencesDialog::indexGridSliderMoved(int value)
{
	SettingsManager::instance()->setValue("IndexGrid", value);
	emit gridChanged();
}

PreferencesDialog::~PreferencesDialog()
{
	SettingsManager::instance()->setValue("currentPreferencesTab", ui->listWidget->currentRow());


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
	mEditorUi->embeddedLinkerIndentSlider->setValue(SettingsManager::instance()->value("EmbeddedLinkerIndent", 8).toInt());
	mEditorUi->embeddedLinkerSizeSlider->setValue(SettingsManager::instance()->value("EmbeddedLinkerSize", 6).toInt());
	mEditorUi->gridWidthSlider->setValue(SettingsManager::instance()->value("GridWidth", 10).toInt());
	mEditorUi->indexGridSlider->setValue(SettingsManager::instance()->value("IndexGrid", 30).toInt());
	mEditorUi->zoomFactorSlider->setValue(SettingsManager::instance()->value("zoomFactor", 2).toInt());
	mWithGrid = mEditorUi->gridWidthSlider->value();
	mIndexGrid = mEditorUi->indexGridSlider->value();

	mBehaviourUi->chooseDiagramsToSaveCheckBox->setChecked(SettingsManager::instance()->value("ChooseDiagramsToSave", true).toBool());
	mBehaviourUi->diagramCreateCheckBox->setChecked(SettingsManager::instance()->value("DiagramCreateSuggestion", true).toBool());
	mBehaviourUi->paletteTabCheckBox->setChecked(SettingsManager::instance()->value("PaletteTabSwitching", true).toBool());
	mMiscellaniousUi->chaoticEditionCheckBox->setChecked(SettingsManager::instance()->value("ChaoticEdition", false).toBool());
	mBehaviourUi->saveExitCheckBox->setChecked(SettingsManager::instance()->value("SaveExitSuggestion", true).toBool());
	mEditorUi->showGridCheckBox->setChecked(SettingsManager::instance()->value("ShowGrid", true).toBool());
	mEditorUi->showAlignmentCheckBox->setChecked(SettingsManager::instance()->value("ShowAlignment", true).toBool());
	mEditorUi->activateGridCheckBox->setChecked(SettingsManager::instance()->value("ActivateGrid", false).toBool());
	mEditorUi->activateAlignmentCheckBox->setChecked(SettingsManager::instance()->value("ActivateAlignment", true).toBool());
	mMiscellaniousUi->antialiasingCheckBox->setChecked(SettingsManager::instance()->value("Antialiasing", true).toBool());
	mMiscellaniousUi->splashScreenCheckBox->setChecked(SettingsManager::instance()->value("Splashscreen", true).toBool());
	mMiscellaniousUi->openGLCheckBox->setChecked(SettingsManager::instance()->value("OpenGL", true).toBool());
	mMiscellaniousUi->squareLineModeCheckBox->setChecked(SettingsManager::instance()->value("SquareLine", true).toBool());

	mBehaviourUi->warningWindowBox->setChecked(SettingsManager::instance()->value("warningWindow", true).toBool());

	mCompilerUi->windowsButton->setChecked(SettingsManager::instance()->value("windowsButton", false).toBool());
	mCompilerUi->linuxButton->setChecked(SettingsManager::instance()->value("linuxButton", false).toBool());
	mCompilerUi->otherButton->setChecked(SettingsManager::instance()->value("otherButton", false).toBool());

	mCompilerUi->pathToQmake->setText(SettingsManager::instance()->value("pathToQmake", "").toString());
	mCompilerUi->pathToMake->setText(SettingsManager::instance()->value("pathToMake", "").toString());
	mCompilerUi->pluginExtension->setText(SettingsManager::instance()->value("pluginExtension", "").toString());
	mCompilerUi->prefix->setText(SettingsManager::instance()->value("prefix", "").toString());

	mDebuggerUi->timeoutLineEdit->setText(SettingsManager::instance()->value("debuggerTimeout", 750).toString());
	mDebuggerUi->colorComboBox->addItems(QColor::colorNames());
	QString curColor = SettingsManager::instance()->value("debugColor", "red").toString();
	int curColorIndex = mDebuggerUi->colorComboBox->findText(curColor);
	mDebuggerUi->colorComboBox->setCurrentIndex(curColorIndex);
	SettingsManager::instance()->value("debugColor", mDebuggerUi->colorComboBox->currentText());
}

void PreferencesDialog::applyChanges()
{
	SettingsManager::instance()->setValue("EmbeddedLinkerIndent", mEditorUi->embeddedLinkerIndentSlider->value());
	SettingsManager::instance()->setValue("EmbeddedLinkerSize", mEditorUi->embeddedLinkerSizeSlider->value());
	SettingsManager::instance()->setValue("GridWidth", mEditorUi->gridWidthSlider->value());
	SettingsManager::instance()->setValue("IndexGrid", mEditorUi->indexGridSlider->value());
	SettingsManager::instance()->setValue("zoomFactor", mEditorUi->zoomFactorSlider->value());
	mWithGrid = mEditorUi->gridWidthSlider->value();
	mIndexGrid = mEditorUi->indexGridSlider->value();

	SettingsManager::instance()->setValue("ChooseDiagramsToSave", mBehaviourUi->chooseDiagramsToSaveCheckBox->isChecked());
	SettingsManager::instance()->setValue("DiagramCreateSuggestion", mBehaviourUi->diagramCreateCheckBox->isChecked());
	SettingsManager::instance()->setValue("PaletteTabSwitching", mBehaviourUi->paletteTabCheckBox->isChecked());
	SettingsManager::instance()->setValue("ChaoticEdition", mMiscellaniousUi->chaoticEditionCheckBox->isChecked());
	SettingsManager::instance()->setValue("SaveExitSuggestion", mBehaviourUi->saveExitCheckBox->isChecked());
	SettingsManager::instance()->setValue("Splashscreen", mMiscellaniousUi->splashScreenCheckBox->isChecked());
	SettingsManager::instance()->setValue("ShowGrid", mEditorUi->showGridCheckBox->isChecked());
	SettingsManager::instance()->setValue("ShowAlignment", mEditorUi->showAlignmentCheckBox->isChecked());
	SettingsManager::instance()->setValue("ActivateGrid", mEditorUi->activateGridCheckBox->isChecked());
	SettingsManager::instance()->setValue("ActivateAlignment", mEditorUi->activateAlignmentCheckBox->isChecked());
	SettingsManager::instance()->setValue("Antialiasing", mMiscellaniousUi->antialiasingCheckBox->isChecked());
	SettingsManager::instance()->setValue("OpenGL", mMiscellaniousUi->openGLCheckBox->isChecked());
	SettingsManager::instance()->setValue("SquareLine", mMiscellaniousUi->squareLineModeCheckBox->isChecked());

	SettingsManager::instance()->setValue("warningWindow", mBehaviourUi->warningWindowBox->isChecked());

	SettingsManager::instance()->setValue("windowsButton", mCompilerUi->windowsButton->isChecked());
	SettingsManager::instance()->setValue("linuxButton", mCompilerUi->linuxButton->isChecked());
	SettingsManager::instance()->setValue("otherButton", mCompilerUi->otherButton->isChecked());

	SettingsManager::instance()->setValue("pathToQmake", mCompilerUi->pathToQmake->text());
	SettingsManager::instance()->setValue("pathToMake", mCompilerUi->pathToMake->text());
	SettingsManager::instance()->setValue("pluginExtension", mCompilerUi->pluginExtension->text());
	SettingsManager::instance()->setValue("prefix", mCompilerUi->prefix->text());

	SettingsManager::instance()->setValue("debuggerTimeout", mDebuggerUi->timeoutLineEdit->text());
	SettingsManager::instance()->setValue("debugColor", mDebuggerUi->colorComboBox->currentText());

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

	SettingsManager::instance()->setValue("GridWidth", mWithGrid);
	SettingsManager::instance()->setValue("IndexGrid", mIndexGrid);
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
