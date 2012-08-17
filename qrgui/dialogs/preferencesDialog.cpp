#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"
#include "preferencesPages/behaviourPage.h"
#include "preferencesPages/debuggerPage.h"
#include "preferencesPages/editorPage.h"
#include "preferencesPages/miscellaniousPage.h"
#include "preferencesPages/featuresPage.h"

PreferencesDialog::PreferencesDialog(QWidget *parent)
		: QDialog(parent)
		, mUi(new Ui::PreferencesDialog)
{
	mUi->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
	SettingsManager::setValue("currentPreferencesTab", mUi->listWidget->currentRow());
	delete mUi;
}

void PreferencesDialog::init(QAction * const showGridAction
		, QAction * const showAlignmentAction
		, QAction * const activateGridAction
		, QAction * const activateAlignmentAction)
{
	PreferencesPage *behaviourPage = new PreferencesBehaviourPage(mUi->pageContentWigdet);
	PreferencesPage *debuggerPage = new PreferencesDebuggerPage(mUi->pageContentWigdet);
	PreferencesPage *miscellaniousPage = new PreferencesMiscellaniousPage(mUi->pageContentWigdet);
//	PreferencesPage *featuresPage = new PreferencesFeaturesPage(ui->pageContentWigdet);
	PreferencesPage *editorPage = new PreferencesEditorPage(showGridAction
		, showAlignmentAction, activateGridAction
		, activateAlignmentAction, mUi->pageContentWigdet);

	connect(mUi->listWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(chooseTab(const QModelIndex &)));
	connect(mUi->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(mUi->okButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
	connect(mUi->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));

	connect(editorPage, SIGNAL(gridChanged()), this, SIGNAL(gridChanged()));
	connect(editorPage, SIGNAL(fontChanged()), this, SIGNAL(fontChanged()));
	connect(editorPage, SIGNAL(paletteRepresentationChanged()), this,
		SIGNAL(paletteRepresentationChanged()));
	connect(miscellaniousPage, SIGNAL(iconsetChanged()), this, SIGNAL(iconsetChanged()));

	registerPage(tr("Behaviour"), behaviourPage);
	registerPage(tr("Debugger"), debuggerPage);
	registerPage(tr("Miscellanious"), miscellaniousPage);
	registerPage(tr("Editor"), editorPage);

	int const currentTab = SettingsManager::value("currentPreferencesTab").toInt();
	mUi->listWidget->setCurrentRow(currentTab);
	chooseTab(mUi->listWidget->currentIndex());
}

void PreferencesDialog::applyChanges()
{
	foreach (PreferencesPage *page, mCustomPages.values()) {
		page->save();
	}

	emit settingsApplied();
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		foreach (PreferencesPage *page, mCustomPages.values()) {
			page->changeEvent(e);
		}
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

void PreferencesDialog::chooseTab(QModelIndex const &index)
{
	mUi->listWidget->setCurrentRow(index.row());
	mUi->pageContentWigdet->setCurrentIndex(index.row() + 1);
}

void PreferencesDialog::registerPage(QString const &pageName, PreferencesPage * const page)
{
	mUi->pageContentWigdet->addWidget(page);
	mCustomPages.insert(pageName, page);
	mUi->listWidget->addItem(new QListWidgetItem(QIcon(page->icon()), pageName));
}

void PreferencesDialog::switchCurrentTab(QString const &tabName)
{
	if (mCustomPages.contains(tabName)) {
		int const currentIndex = mCustomPages.keys().indexOf(tabName);
		mUi->listWidget->setCurrentRow(currentIndex);
		mUi->pageContentWigdet->setCurrentIndex(currentIndex + 1);
	}
}
void PreferencesDialog::changePaletteParameters()
{
	if (mCustomPages.count(tr("Editor")) > 0) {
		static_cast<PreferencesEditorPage*>(mCustomPages[tr("Editor")])->changePaletteParameters();
	}
}
