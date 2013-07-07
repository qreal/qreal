#pragma once

#include <QtCore/QObject>
#include <QtCore/QFileInfo>

#include "../textEditor/codeArea.h"
#include "systemEventsInterface.h"

namespace qReal {

class Id;

class SystemEvents : public SystemEventsInterface
{
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

	void emitCodePathChanged(CodeArea *area, QFileInfo const &fileInfo)
	{
		emit codePathChanged(area, fileInfo);
	}
};
}
