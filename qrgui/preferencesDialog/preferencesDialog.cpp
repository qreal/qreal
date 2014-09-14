#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include <QtWidgets/QMessageBox>

#include <qrutils/qRealFileDialog.h>

#include "preferencesPages/behaviourPage.h"
#include "preferencesPages/debuggerPage.h"
#include "preferencesPages/editorPage.h"
#include "preferencesPages/miscellaniousPage.h"
#include "preferencesPages/featuresPage.h"
#include "hotKeyManager/hotKeyManagerPage.h"

using namespace qReal::gui;
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

void PreferencesDialog::init()
{
	PreferencesPage *behaviourPage = new PreferencesBehaviourPage(mUi->pageContentWidget);
	// Debugger page removed due to #736
	PreferencesMiscellaniousPage *miscellaniousPage = new PreferencesMiscellaniousPage(mUi->pageContentWidget);
	PreferencesPage *editorPage = new PreferencesEditorPage(mUi->pageContentWidget);
	PreferencesPage *hotKeyManagerPage = new PreferencesHotKeyManagerPage(mUi->pageContentWidget);

	connect(mUi->listWidget, &QListWidget::clicked, this, &PreferencesDialog::chooseTab);
	connect(mUi->listWidget, &QListWidget::activated, this, &PreferencesDialog::chooseTab);
	connect(mUi->applyButton, &QAbstractButton::clicked, this, &PreferencesDialog::applyChanges);
	connect(mUi->okButton, &QAbstractButton::clicked, this, &PreferencesDialog::saveAndClose);
	connect(mUi->cancelButton, &QAbstractButton::clicked, this, &PreferencesDialog::cancel);
	connect(mUi->exportButton, &QAbstractButton::clicked, this, &PreferencesDialog::exportSettings);
	connect(mUi->importButton, &QAbstractButton::clicked, this, &PreferencesDialog::importSettings);

	registerPage(tr("Behaviour"), behaviourPage);
	registerPage(tr("Miscellanious"), miscellaniousPage);
	registerPage(tr("Editor"), editorPage);
	registerPage(tr("Shortcuts"), hotKeyManagerPage);

	int const currentTab = SettingsManager::value("currentPreferencesTab").toInt();
	mUi->listWidget->setCurrentRow(currentTab);
	chooseTab(mUi->listWidget->currentIndex());
}

//void PreferencesDialog::updatePluginDependendSettings()
//{
//	setWindowIcon(BrandManager::applicationIcon());
//}

void PreferencesDialog::applyChanges()
{
	bool shouldRestart = false;
	for (PreferencesPage * const page : mCustomPages.values()) {
		page->save();
		shouldRestart |= page->mShouldRestartSystemToApply;
		page->mShouldRestartSystemToApply = false;
	}

	SettingsManager::instance()->saveData();

	if (shouldRestart) {
		QMessageBox::information(this, tr("Information"), tr("You should restart the system to apply changes"));
	}

	emit settingsApplied();
}

void PreferencesDialog::restoreSettings()
{
	for (PreferencesPage * const page : mCustomPages.values()) {
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
	mUi->pageContentWidget->setCurrentIndex(index.row() + 1);
}

void PreferencesDialog::registerPage(QString const &pageName, PreferencesPage * const page)
{
	mUi->pageContentWidget->addWidget(page);
	mCustomPages.insert(pageName, page);
	mPagesIndexes.insert(pageName, mCustomPages.count() - 1);
	mUi->listWidget->addItem(new QListWidgetItem(QIcon(page->windowIcon()), pageName));
}

void PreferencesDialog::switchCurrentPage(QString const &tabName)
{
	if (mCustomPages.contains(tabName)) {
		int const currentIndex = mPagesIndexes[tabName];
		mUi->listWidget->setCurrentRow(currentIndex);
		mUi->pageContentWidget->setCurrentIndex(currentIndex + 1);
	}
}
void PreferencesDialog::exportSettings()
{
	QString fileNameForExport = QRealFileDialog::getSaveFileName("SaveEnginePreferences", this
			, tr("Save File"), "/mySettings", tr("*.ini"));
	if (!fileNameForExport.endsWith(".ini")) {
		fileNameForExport += ".ini";
	}

	SettingsManager::instance()->saveSettings(fileNameForExport);
}

void PreferencesDialog::importSettings()
{
	QString fileNameForImport = QRealFileDialog::getOpenFileName("OpenEnginePreferences", this
			, tr("Open File"), "/mySettings", tr("*.ini"));
	SettingsManager::instance()->loadSettings(fileNameForImport);
}
