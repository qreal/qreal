#pragma once

#include <QtCore/QString>
#include <QtWidgets/QAction>

namespace qReal {

class HotKeyActionInfo
{
public:
	HotKeyActionInfo(QString const &id, QString const &label, QAction * const action)
		: mAction(action)
		, mId(id)
		, mLabel(label)
	{
	}

	QAction *action() const
	{
		return mAction;
	}

	QString id() const
	{
		return mId;
	}

	QString label() const
	{
		return mLabel;
	}

private:
	QAction *mAction;
	QString mId;
	QString mLabel;
};

}

