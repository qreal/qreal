#include "hotKeyManagerPage.h"
#include "ui_hotKeyManagerPage.h"
#include "hotKeyManager.h"

#include <QtWidgets/QAction>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtGui/QKeySequence>
#include <QtCore/QDebug>
#include <QtGui/QWheelEvent>

using namespace qReal;

PreferencesHotKeyManagerPage:: PreferencesHotKeyManagerPage(QWidget *parent)
	: PreferencesPage(parent)
	, mUi(new Ui::hotKeyManagerPage)
	, mCurrentRow(-1)
	, mCurrentModifiers(Qt::NoModifier)
{
	mUi->setupUi(this);

	mUi->hotKeysTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	connect(mUi->hotKeysTable, SIGNAL(cellEntered(int,int)), this, SLOT(activateShortcutLineEdit(int,int)));
	connect(mUi->hotKeysTable, SIGNAL(cellClicked(int,int)), this, SLOT(activateShortcutLineEdit(int,int)));
	connect(mUi->shortcutLineEdit, SIGNAL(newModifiers(Qt::KeyboardModifiers)), this, SLOT(newModifiers(Qt::KeyboardModifiers)));
	connect(mUi->shortcutLineEdit, SIGNAL(newKey(int)), this, SLOT(newKey(int)));

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

void PreferencesHotKeyManagerPage::mousePressEvent(QMouseEvent *event)
{
	/*MouseButtons mb = None;

	switch(event->button()) {
		case Qt::RightButton:
			mb = MouseRB;
			qDebug() << "RB";
			break;
		case Qt::LeftButton:
			mb = MouseLB;
			qDebug() << "LB";
			break;
		case Qt::MiddleButton:
			mb = MouseMB;
			qDebug() << "MB";
			break;
		default:
			return;
	}

	if (mCurrentRow != -1 && mCurrentModifiers != "") {
		if (HotKeyManager::setShortcut
					(mUi->hotKeysTable->item(mCurrentRow, 0)->text()
					 , QKeySequence(mCurrentModifiers), mb)) {
			updateCurrentRow(HotKeyManager::sequence(mCurrentModifiers, mb));
		}
		mUi->shortcutLineEdit->setFocus();
		mCurrentModifiers = "";
	}*/
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

void PreferencesHotKeyManagerPage::activateShortcutLineEdit(int const row, int const column)
{
	mCurrentRow = row;

	mUi->shortcutLineEdit->setText(mUi->hotKeysTable->item(row, 2)->text());
	mUi->shortcutLineEdit->setEnabled(true);
}

void PreferencesHotKeyManagerPage::newModifiers(Qt::KeyboardModifiers modifiers)
{
	mCurrentModifiers = modifiers;
}

void PreferencesHotKeyManagerPage::newKey(int const key)
{
	if (mCurrentRow != -1) {
		if (HotKeyManager::setShortcut
					(mUi->hotKeysTable->item(mCurrentRow, 0)->text()
					 , QKeySequence(mCurrentModifiers + key))) {
			updateCurrentRow(QKeySequence(mCurrentModifiers + key).toString());
		}
		mCurrentModifiers = Qt::NoModifier;
	}
}

void PreferencesHotKeyManagerPage::updateCurrentRow(QString const shortcut)
{
	QTableWidgetItem *item = mUi->hotKeysTable->item(mCurrentRow, 2);
	item->setText(item->text() + shortcut + ", ");
	mUi->shortcutLineEdit->setText(item->text());
}
