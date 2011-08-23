#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include "preferencesPages/behaviourPage.h"
#include "preferencesPages/compilerPage.h"
#include "preferencesPages/debuggerPage.h"
#include "preferencesPages/editorPage.h"
#include "preferencesPages/miscellaniousPage.h"
#include "preferencesPages/featuresPage.h"

PreferencesDialog::PreferencesDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::PreferencesDialog)
	, mBehaviourPage(NULL)
	, mCompilerPage(NULL)
	, mDebuggerPage(NULL)
	, mEditorPage(NULL)
	, mMiscellaniousPage(NULL)
	, mFeaturesPage(NULL)
{
	ui->setupUi(this);
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
	delete mFeaturesPage;

//	foreach (PreferencesPage *page, mCustomPages.values())
//		delete page;
}

void PreferencesDialog::init(QAction * const showGridAction, QAction * const showAlignmentAction
	,QAction * const activateGridAction, QAction * const activateAlignmentAction)
{
	mBehaviourPage = new PreferencesBehaviourPage(ui->pageContentWigdet);
	mCompilerPage = new PreferencesCompilerPage(ui->pageContentWigdet);
	mDebuggerPage = new PreferencesDebuggerPage(ui->pageContentWigdet);
	mMiscellaniousPage = new PreferencesMiscellaniousPage(ui->pageContentWigdet);
	mFeaturesPage = new PreferencesFeaturesPage(ui->pageContentWigdet);
	mEditorPage = new PreferencesEditorPage(showGridAction,
		showAlignmentAction, activateGridAction, activateAlignmentAction, ui->pageContentWigdet);

	connect(ui->listWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(chooseTab(const QModelIndex &)));
	connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	connect(mEditorPage, SIGNAL(gridChanged()), this, SIGNAL(gridChanged()));
	connect(mMiscellaniousPage, SIGNAL(iconsetChanged()), this, SIGNAL(iconsetChanged()));

	connect(mEditorPage, SIGNAL(fontChanged()), this, SIGNAL(fontChanged()));

	int currentTab = SettingsManager::value("currentPreferencesTab", 0).toInt();
	ui->listWidget->setCurrentRow(currentTab);
	chooseTab(ui->listWidget->currentIndex());
}

void PreferencesDialog::applyChanges()
{
	mBehaviourPage->save();
	mCompilerPage->save();
	mEditorPage->save();
	mDebuggerPage->save();
	mMiscellaniousPage->save();
	mFeaturesPage->save();

	foreach (PreferencesPage *page, mCustomPages.values())
		page->save();

	emit settingsApplied();
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);

		if (mBehaviourPage) {
			mBehaviourPage->changeEvent(e);
			mCompilerPage->changeEvent(e);
			mDebuggerPage->changeEvent(e);
			mEditorPage->changeEvent(e);
			mMiscellaniousPage->changeEvent(e);
			mFeaturesPage->changeEvent(e);
		}
		foreach (PreferencesPage *page, mCustomPages.values())
			page->changeEvent(e);
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
	hidePages();

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

//	case features:
//		mFeaturesPage->show();
//		break;
	default:
		QString const name = index.data().toString();
		if (mCustomPages.contains(name))
			mCustomPages[name]->show();
	}
}

void PreferencesDialog::hidePages()
{
	mBehaviourPage->hide();
	mCompilerPage->hide();
	mEditorPage->hide();
	mDebuggerPage->hide();
	mMiscellaniousPage->hide();
	mFeaturesPage->hide();

	foreach (PreferencesPage *page, mCustomPages.values())
		page->hide();
}

void PreferencesDialog::registerPage(QString const &pageName, PreferencesPage * const page)
{
	page->setParent(ui->pageContentWigdet);
	mCustomPages.insert(pageName, page);
	ui->listWidget->addItem(pageName);
}

void PreferencesDialog::switchCurrentTab(QString const &tabName)
{
	if (mCustomPages.contains(tabName)) {
		ui->listWidget->setCurrentRow(mCustomPages.keys().indexOf(tabName) + debugger + 1);
		hidePages();
		mCustomPages[tabName]->show();
	}
}
