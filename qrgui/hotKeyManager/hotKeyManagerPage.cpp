#include <QtWidgets/QAction>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtGui/QKeySequence>
#include <QtCore/QDebug>
#include <QtGui/QWheelEvent>

#include "hotKeyManagerPage.h"
#include "ui_hotKeyManagerPage.h"
#include "hotKeyManager.h"

using namespace qReal;

PreferencesHotKeyManagerPage:: PreferencesHotKeyManagerPage(QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::hotKeyManagerPage)
	, mCurrentId("")
	, mCurrentItem(NULL)
	, mCurrentModifiers(Qt::NoModifier)
{
	mUi->setupUi(this);

	mUi->hotKeysTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	connect(mUi->hotKeysTable, SIGNAL(cellClicked(int,int)), this, SLOT(activateShortcutLineEdit(int,int)));
	connect(mUi->hotKeysTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(doubleClicked(int,int)));
	connect(mUi->shortcutLineEdit, SIGNAL(newModifiers(Qt::KeyboardModifiers)), this, SLOT(newModifiers(Qt::KeyboardModifiers)));
	connect(mUi->shortcutLineEdit, SIGNAL(newKey(int)), this, SLOT(newKey(int)));
	connect(mUi->resetShortcutPushButton, SIGNAL(pressed()), this, SLOT(resetShortcuts()));
	connect(mUi->resetAllPushButton, SIGNAL(pressed()), this, SLOT(resetAllShortcuts()));

	initTable();
	loadHotKeys();
}

PreferencesHotKeyManagerPage::~PreferencesHotKeyManagerPage()
{
	delete mUi;
}

void PreferencesHotKeyManagerPage::save()
{

}

void PreferencesHotKeyManagerPage::restoreSettings()
{

}

void PreferencesHotKeyManagerPage::resetShortcuts()
{
	if (mCurrentId != "") {
		HotKeyManager::deleteShortcut(mCurrentId, mCurrentItem->text());
		mCurrentItem->setText("");
		mUi->shortcutLineEdit->setText("");
	}
}

void PreferencesHotKeyManagerPage::resetAllShortcuts()
{
	HotKeyManager::resetAllShortcuts();
	mUi->hotKeysTable->clearContents();
	initTable();
	loadHotKeys();
}

void PreferencesHotKeyManagerPage::loadHotKeys()
{
	QHash<QString, QAction *> cmds = HotKeyManager::commands();
	QHash<QString, QString> shortcuts = HotKeyManager::shortcuts();

	QHash<QString, QAction *>::iterator i;
	int k;
	for (i = cmds.begin(), k = 0; i != cmds.end(); ++i, ++k) {
		QStringList sequences = shortcuts.keys(i.key());

		mUi->hotKeysTable->item(k, 0)->setText(i.key());
		mUi->hotKeysTable->item(k, 1)->setText(i.value()->whatsThis());

		int j = 0;

		foreach (QString const &sequence, sequences) {
			mUi->hotKeysTable->item(k, 2 + j)->setText(sequence);
			j++;
			if (j == 3) {
				break;
			}
		}
	}
}

void PreferencesHotKeyManagerPage::initTable()
{
	int const rows = HotKeyManager::commands().size();

	mUi->hotKeysTable->setRowCount(rows);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < 5; ++j) {
			mUi->hotKeysTable->setItem(i, j, new QTableWidgetItem(""));
		}
	}
}

void PreferencesHotKeyManagerPage::doubleClicked(const int row, const int column)
{
	mUi->shortcutLineEdit->setFocus();
	activateShortcutLineEdit(row, column);
}

void PreferencesHotKeyManagerPage::activateShortcutLineEdit(int const row, int const column)
{
	if (column > 1) {
		mCurrentId = mUi->hotKeysTable->item(row, 0)->text();
		mCurrentItem = mUi->hotKeysTable->item(row, column);

		mUi->shortcutLineEdit->setText(mCurrentItem->text());
		mUi->shortcutLineEdit->setEnabled(true);

		setTextColor(mCurrentItem->textColor());

		if (HotKeyManager::setShortcut(mCurrentId, mCurrentItem->text())) {
			setTextColor(Qt::black);
		}
	} else {
		mCurrentId = "";
		mUi->shortcutLineEdit->clear();
		mUi->shortcutLineEdit->setEnabled(false);
	}
}

void PreferencesHotKeyManagerPage::newModifiers(Qt::KeyboardModifiers modifiers)
{
	mCurrentModifiers = modifiers;
}

void PreferencesHotKeyManagerPage::newKey(int const key)
{
	if (mCurrentId != "") {
		if (mCurrentItem->text() == "") {
			if (HotKeyManager::setShortcut(mCurrentId, QKeySequence(mCurrentModifiers + key))) {
				setTextColor(Qt::black);
			} else {
				setTextColor(Qt::red);
			}
			mCurrentItem->setText(QKeySequence(mCurrentModifiers + key).toString());
			mUi->shortcutLineEdit->setText(mCurrentItem->text());
		} else {
			int const parts = mCurrentItem->text().count(',');

			if (parts != 3) {
				QString const shortcut = QString("%1, %2").arg(
									mCurrentItem->text()
									, QKeySequence(mCurrentModifiers + key).toString()
								);

				if (mCurrentItem->textColor() == Qt::black) {
					HotKeyManager::deleteShortcut(mCurrentId, mCurrentItem->text());
				}

				if (HotKeyManager::setShortcut(mCurrentId, QKeySequence(shortcut))) {
					setTextColor(Qt::black);
					mCurrentItem->setText(shortcut);
					mUi->shortcutLineEdit->setText(mCurrentItem->text());
				} else {
					HotKeyManager::setShortcut(mCurrentId,  mCurrentItem->text());

					if (parts < 2) {
						setTextColor(Qt::red);
						mCurrentItem->setText(shortcut);
						mUi->shortcutLineEdit->setText(mCurrentItem->text());
					}
				}
			}
		}
		mCurrentModifiers = Qt::NoModifier;
	}
}

void PreferencesHotKeyManagerPage::setTextColor(QColor const &color)
{
	mCurrentItem->setTextColor(color);
	QPalette palette;
	palette.setColor(QPalette::Text, color);
	mUi->shortcutLineEdit->setPalette(palette);
}
