#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include "preferencesPages/behaviourPage.h"
#include "preferencesPages/compilerPage.h"
#include "preferencesPages/debuggerPage.h"
#include "preferencesPages/editorPage.h"
#include "preferencesPages/miscellaniousPage.h"
#include "preferencesPages/robotSettingsPage.h"

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
