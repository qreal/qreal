#include "hotKeyManagerPage.h"
#include "ui_hotKeyManagerPage.h"
#include "hotKeyManager.h"

#include <QtWidgets/QAction>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtGui/QKeySequence>
#include <QtCore/QDebug>

using namespace qReal;

PreferencesHotKeyManagerPage:: PreferencesHotKeyManagerPage(QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::hotKeyManagerPage)
{
	mUi->setupUi(this);

	mUi->hotKeysTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	connect(mUi->hotKeysTable, SIGNAL(cellEntered(int,int)), this, SLOT(activateShortcutLineEdit(int,int)));
	connect(mUi->hotKeysTable, SIGNAL(cellClicked(int,int)), this, SLOT(activateShortcutLineEdit(int,int)));
	connect(mUi->shortcutLineEdit, SIGNAL(newShortcut(QKeySequence)), this, SLOT(newShortcut(QKeySequence)));

	loadHotKeys();
}

PreferencesHotKeyManagerPage::~ PreferencesHotKeyManagerPage()
{
	delete mUi;
}

void PreferencesHotKeyManagerPage::save()
{

}

void PreferencesHotKeyManagerPage::restoreSettings()
{

}

void PreferencesHotKeyManagerPage::loadHotKeys()
{
	QHash<QString, QAction *> cmds = HotKeyManager::commands();
	QHash<QString, QString> shortcuts = HotKeyManager::cmdsShortcuts();

	QHash<QString, QAction *>::iterator i;
	int k;

	for (i = cmds.begin(), k = 0; i != cmds.end(); ++i, ++k) {
		QString sc;
		QList<QString> mseqs = shortcuts.keys(i.key());

		foreach(QString mseq, mseqs) {
			sc += mseq + ", ";
		}

		mUi->hotKeysTable->setRowCount(k + 1);

		mUi->hotKeysTable->setItem(k, 0, new QTableWidgetItem(i.key()));
		mUi->hotKeysTable->setItem(k, 1, new QTableWidgetItem(i.value()->whatsThis()));
		mUi->hotKeysTable->setItem(k, 2, new QTableWidgetItem(sc));
	}
}

void PreferencesHotKeyManagerPage::activateShortcutLineEdit(int row, int column)
{
	mCurrentRow = row;

	mUi->shortcutLineEdit->setText(mUi->hotKeysTable->item(row, 2)->text());
	mUi->shortcutLineEdit->setEnabled(true);
}

void PreferencesHotKeyManagerPage::newShortcut(QKeySequence keyseq)
{
	qDebug() << keyseq.toString();
}
