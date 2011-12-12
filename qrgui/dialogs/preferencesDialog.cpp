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
{
	ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
	SettingsManager::setValue("currentPreferencesTab", ui->listWidget->currentRow());

	foreach (PreferencesPage *page, mCustomPages.values()) {
		page->setParent(NULL);
		delete page;
	}

	delete ui;
}

void PreferencesDialog::init(QAction * const showGridAction, QAction * const showAlignmentAction
	,QAction * const activateGridAction, QAction * const activateAlignmentAction)
{
	PreferencesPage *behaviourPage = new PreferencesBehaviourPage(ui->pageContentWigdet);
	PreferencesPage *compilerPage = new PreferencesCompilerPage(ui->pageContentWigdet);
	PreferencesPage *debuggerPage = new PreferencesDebuggerPage(ui->pageContentWigdet);
	PreferencesPage *miscellaniousPage = new PreferencesMiscellaniousPage(ui->pageContentWigdet);
//	PreferencesPage *featuresPage = new PreferencesFeaturesPage(ui->pageContentWigdet);
	PreferencesPage *editorPage = new PreferencesEditorPage(showGridAction
		, showAlignmentAction, activateGridAction, activateAlignmentAction, ui->pageContentWigdet);

	connect(ui->listWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(chooseTab(const QModelIndex &)));
	connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	connect(editorPage, SIGNAL(gridChanged()), this, SIGNAL(gridChanged()));
	connect(editorPage, SIGNAL(fontChanged()), this, SIGNAL(fontChanged()));
	connect(miscellaniousPage, SIGNAL(iconsetChanged()), this, SIGNAL(iconsetChanged()));

	registerPage(tr("Behaviour"), behaviourPage);
	registerPage(tr("Compiler"), compilerPage);
	registerPage(tr("Debugger"), debuggerPage);
	registerPage(tr("Miscellanious"), miscellaniousPage);
	registerPage(tr("Editor"), editorPage);

	int currentTab = SettingsManager::value("currentPreferencesTab", 0).toInt();
	ui->listWidget->setCurrentRow(currentTab);
	chooseTab(ui->listWidget->currentIndex());
}

void PreferencesDialog::applyChanges()
{
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
	QString const name = index.data().toString();
	if (mCustomPages.contains(name)) {
		mCustomPages[name]->show();
	}
}


void PreferencesDialog::hidePages()
{
	foreach (PreferencesPage *page, mCustomPages.values())
		page->hide();
}

void PreferencesDialog::registerPage(QString const &pageName, PreferencesPage * const page)
{
	page->setParent(ui->pageContentWigdet);
	mCustomPages.insert(pageName, page);
	ui->listWidget->addItem(new QListWidgetItem(QIcon(page->getIcon()), pageName));
}

void PreferencesDialog::switchCurrentTab(QString const &tabName)
{
	if (mCustomPages.contains(tabName)) {
		ui->listWidget->setCurrentRow(mCustomPages.keys().indexOf(tabName));  // count of current pages
		hidePages();
		mCustomPages[tabName]->show();
	}
}
