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
		qDebug() << Qt::SHIFT + Qt::ALT;
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
