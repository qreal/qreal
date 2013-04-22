#pragma once

#include <QLineEdit>
#include <QtGui/QKeySequence>

class ShortcutEdit : public QLineEdit
{
	Q_OBJECT
public:
	explicit ShortcutEdit(QObject *parent = 0);

	bool event(QEvent * event);
	
signals:
	void newShortcut(QKeySequence keyseq);
};
