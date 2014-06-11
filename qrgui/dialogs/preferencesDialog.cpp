#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include <qrutils/qRealFileDialog.h>

#include "dialogs/preferencesPages/behaviourPage.h"
#include "dialogs/preferencesPages/debuggerPage.h"
#include "dialogs/preferencesPages/editorPage.h"
#include "dialogs/preferencesPages/miscellaniousPage.h"
#include "dialogs/preferencesPages/featuresPage.h"
#include "hotKeyManager/hotKeyManagerPage.h"
#include "brandManager/brandManager.h"

using namespace qReal;
using namespace utils;

PreferencesDialog::PreferencesDialog(QWidget *parent)
	: QRealDialog("PreferencesDialog", parent)
	, mUi(new Ui::PreferencesDialog)
{
	mUi->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
	SettingsManager::setValue("currentPreferencesTab", mUi->listWidget->currentRow());
	delete mUi;
}

void PreferencesDialog::init(QAction * const showGridAction, QAction * const showAlignmentAction
	, QAction * const activateGridAction, QAction * const activateAlignmentAction)
{
	PreferencesPage *behaviourPage = new PreferencesBehaviourPage(mUi->pageContentWigdet);
	// Debugger page removed due to #736
	PreferencesMiscellaniousPage *miscellaniousPage = new PreferencesMiscellaniousPage(mUi->pageContentWigdet);
	PreferencesPage *editorPage = new PreferencesEditorPage(showGridAction
		, showAlignmentAction, activateGridAction, activateAlignmentAction, mUi->pageContentWigdet);
	PreferencesPage *hotKeyManagerPage = new PreferencesHotKeyManagerPage(mUi->pageContentWigdet);

	connect(mUi->listWidget, SIGNAL(clicked(QModelIndex))
			, this, SLOT(chooseTab(const QModelIndex &)));
	connect(mUi->listWidget, SIGNAL(activated(const QModelIndex &))
			, this, SLOT(chooseTab(const QModelIndex &)));
	connect(mUi->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(mUi->okButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
	connect(mUi->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(mUi->exportButton, SIGNAL(clicked()), this, SLOT(exportSettings()));
	connect(mUi->importButton, SIGNAL(clicked()), this, SLOT(importSettings()));

	connect(editorPage, SIGNAL(gridChanged()), this, SIGNAL(gridChanged()));
	connect(editorPage, SIGNAL(fontChanged()), this, SIGNAL(fontChanged()));
	connect(editorPage, SIGNAL(paletteRepresentationChanged()), this
		, SIGNAL(paletteRepresentationChanged()));
	connect(miscellaniousPage, SIGNAL(iconsetChanged()), this, SIGNAL(iconsetChanged()));
	connect(miscellaniousPage, &PreferencesMiscellaniousPage::toolbarSizeChanged
			, this, &PreferencesDialog::toolbarSizeChanged);
	connect(behaviourPage, SIGNAL(usabilityTestingModeChanged(bool))
			, this, SIGNAL(usabilityTestingModeChanged(bool)), Qt::UniqueConnection);

	registerPage(tr("Behaviour"), behaviourPage);
	registerPage(tr("Miscellanious"), miscellaniousPage);
	registerPage(tr("Editor"), editorPage);
	registerPage(tr("Shortcuts"), hotKeyManagerPage);

	int const currentTab = SettingsManager::value("currentPreferencesTab").toInt();
	mUi->listWidget->setCurrentRow(currentTab);
	chooseTab(mUi->listWidget->currentIndex());
}

void PreferencesDialog::updatePluginDependendSettings()
{
	setWindowIcon(BrandManager::applicationIcon());
}

void PreferencesDialog::applyChanges()
{
	foreach (PreferencesPage *page, mCustomPages.values()) {
		page->save();
	}

	SettingsManager::instance()->saveData();
	emit settingsApplied();
}

void PreferencesDialog::restoreSettings()
{
	foreach (PreferencesPage *page, mCustomPages.values()) {
		page->restoreSettings();
	}
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			mUi->retranslateUi(this);
			foreach (PreferencesPage *page, mCustomPages.values())
				page->changeEvent(e);
			break;
		default:
			break;
	}
}

void PreferencesDialog::showEvent(QShowEvent *e)
{
	restoreSettings();
	QRealDialog::showEvent(e);
}

void PreferencesDialog::saveAndClose()
{
	applyChanges();
	close();
}

void PreferencesDialog::cancel()
{
	restoreSettings();
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
	mUi->listWidget->addItem(new QListWidgetItem(QIcon(page->getIcon()), pageName));
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

void PreferencesDialog::exportSettings()
{
	QString fileNameForExport = QRealFileDialog::getSaveFileName("SaveEnginePreferences", this
			, tr("Save File"),"/mySettings",tr("*.ini"));
	if (!fileNameForExport.endsWith(".ini")) {
		fileNameForExport += ".ini";
	}
	SettingsManager::instance()->saveSettings(fileNameForExport);
}

void PreferencesDialog::importSettings()
{
	QString fileNameForImport = QRealFileDialog::getOpenFileName("OpenEnginePreferences", this
			, tr("Open File"),"/mySettings",tr("*.ini"));
	SettingsManager::instance()->loadSettings(fileNameForImport);
}
