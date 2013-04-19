#pragma once

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtGui/QKeySequence>

#include "hotKeyManagerTypes.h"

namespace qReal {

class HotKeyAction : public QObject
{
	Q_OBJECT
public:
	explicit HotKeyAction();
	
	void press();

	void addShortcut(QString const shortcut);
	void addShortcut(QKeySequence keyseq);
	void addShortcut(QKeySequence keyseq, MouseShortcuts mouseShortcut);

	static QString sequence(QKeySequence keyseq, MouseShortcuts msc);
	QList<QString> shortcuts();
	QString description();
	void setDescription(QString const descr);
	void resetShortcuts();

signals:
	void pressed();

private:
	QString mDescription;
	QList<QString> mShortcuts;
};

}
