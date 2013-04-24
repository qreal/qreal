#include "shortcutEdit.h"
#include "QtWidgets/QLineEdit"
#include <QtCore/QEvent>
#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>
#include <QtGui/QKeySequence>


ShortcutEdit::ShortcutEdit(QObject *parent)
	: QLineEdit(static_cast<QWidget *> (parent))
{
}

bool ShortcutEdit::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyevent = static_cast<QKeyEvent *> (event);
		QKeySequence keyseq;
		switch(keyevent->key()) {
			case 16777248: //Shift modifier
			case 16777249: //Ctrl modifier
			case 16777251: //Alt modifier
				emit newModifiers(QKeySequence(keyevent->modifiers()).toString());
				break;
			default:
				emit newKey(QKeySequence(keyevent->key()).toString());
		}
		return true;
	}
	return QLineEdit::event(event);
}
