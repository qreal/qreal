#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include <qrutils/qRealFileDialog.h>

#include "dialogs/preferencesPages/behaviourPage.h"
#include "dialogs/preferencesPages/debuggerPage.h"
#include "dialogs/preferencesPages/editorPage.h"
#include "dialogs/preferencesPages/miscellaniousPage.h"
#include "dialogs/preferencesPages/featuresPage.h"
#include "hotKeyManager/hotKeyManagerPage.h"

using namespace qReal;
using namespace utils;

PreferencesDialog::PreferencesDialog(QWidget *parent)
	: QRealDialog("PreferencesDialog", parent)
	, ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
	SettingsManager::setValue("currentPreferencesTab", ui->listWidget->currentRow());
	delete ui;
}

void PreferencesDialog::init(QAction * const showGridAction, QAction * const showAlignmentAction
	, QAction * const activateGridAction, QAction * const activateAlignmentAction)
{
	PreferencesPage *behaviourPage = new PreferencesBehaviourPage(ui->pageContentWigdet);
	// Debugger page removed due to #736
	PreferencesPage *miscellaniousPage = new PreferencesMiscellaniousPage(ui->pageContentWigdet);
	PreferencesPage *editorPage = new PreferencesEditorPage(showGridAction
		, showAlignmentAction, activateGridAction, activateAlignmentAction, ui->pageContentWigdet);
	PreferencesPage *hotKeyManagerPage = new PreferencesHotKeyManagerPage(ui->pageContentWigdet);

	connect(ui->listWidget, SIGNAL(clicked(QModelIndex))
			, this, SLOT(chooseTab(const QModelIndex &)));
	connect(ui->listWidget, SIGNAL(activated(const QModelIndex &))
			, this, SLOT(chooseTab(const QModelIndex &)));
	connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(exportSettings()));
	connect(ui->importButton, SIGNAL(clicked()), this, SLOT(importSettings()));

	connect(editorPage, SIGNAL(gridChanged()), this, SIGNAL(gridChanged()));
	connect(editorPage, SIGNAL(fontChanged()), this, SIGNAL(fontChanged()));
	connect(editorPage, SIGNAL(paletteRepresentationChanged()), this
		, SIGNAL(paletteRepresentationChanged()));
	connect(miscellaniousPage, SIGNAL(iconsetChanged()), this, SIGNAL(iconsetChanged()));
	connect(behaviourPage, SIGNAL(usabilityTestingModeChanged(bool))
			, this, SIGNAL(usabilityTestingModeChanged(bool)), Qt::UniqueConnection);

	registerPage(tr("Behaviour"), behaviourPage);
	registerPage(tr("Miscellanious"), miscellaniousPage);
	registerPage(tr("Editor"), editorPage);
	registerPage(tr("Shortcuts"), hotKeyManagerPage);

	int const currentTab = SettingsManager::value("currentPreferencesTab").toInt();
	ui->listWidget->setCurrentRow(currentTab);
	chooseTab(ui->listWidget->currentIndex());
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
			ui->retranslateUi(this);
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
	close();
}

void PreferencesDialog::chooseTab(QModelIndex const &index)
{
	ui->listWidget->setCurrentRow(index.row());
	ui->pageContentWigdet->setCurrentIndex(index.row() + 1);
}

void PreferencesDialog::registerPage(QString const &pageName, PreferencesPage * const page)
{
	ui->pageContentWigdet->addWidget(page);
	mCustomPages.insert(pageName, page);
	ui->listWidget->addItem(new QListWidgetItem(QIcon(page->getIcon()), pageName));
}

void PreferencesDialog::switchCurrentTab(QString const &tabName)
{
	if (mCustomPages.contains(tabName)) {
		int const currentIndex = mCustomPages.keys().indexOf(tabName);
		ui->listWidget->setCurrentRow(currentIndex);
		ui->pageContentWigdet->setCurrentIndex(currentIndex + 1);
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
