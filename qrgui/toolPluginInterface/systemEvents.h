#pragma once

#include <QtCore/QObject>

namespace qReal {

class Id;

class SystemEvents : public QObject
{
	Q_OBJECT

public:
	void emitCloseMainWindow()
	{
		emit closedMainWindow();
	}

	void emitActiveTabChanged(Id const & rootElementId)
	{
		emit activeTabChanged(rootElementId);
	}

	void emitSettingsUpdated()
	{
		emit settingsUpdated();
	}

signals:
	void closedMainWindow();
	void activeTabChanged(Id const & rootElementId);
	void settingsUpdated();
	
};
}
