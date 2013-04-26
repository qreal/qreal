#include "QtWidgets/QLineEdit"
#include <QtCore/QEvent>
#include <QtCore/QDebug>
#include <QtGui/QKeyEvent>
#include <QtGui/QKeySequence>

#include "shortcutEdit.h"

ShortcutEdit::ShortcutEdit(QWidget *parent)
	: QLineEdit(parent)
{
}

bool ShortcutEdit::event(QEvent *event)
{
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyevent = static_cast<QKeyEvent *> (event);

		switch(keyevent->key()) {
			case Qt::Key_Shift:
			case Qt::Key_Control:
			case Qt::Key_Alt:
			case Qt::Key_AltGr:
				emit newModifiers(keyevent->modifiers());
				break;
			default:
				emit newKey(keyevent->key());
		}
		return true;
	}
	return QLineEdit::event(event);
}
