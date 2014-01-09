#pragma once

#include <QtCore/QObject>
#include <QtCore/QFileInfo>

#include "systemEventsInterface.h"

namespace qReal {

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

	void emitCodePathChanged(qReal::Id const &diagram, QFileInfo const &oldFileInfo, QFileInfo const &newFileInfo)
	{
		emit codePathChanged(diagram, oldFileInfo, newFileInfo);
	}

	void emitNewCodeAppeared(qReal::Id const &diagram, QFileInfo const &fileInfo)
	{
		emit newCodeAppeared(diagram, fileInfo);
	}

	void emitDiagramClosed(qReal::Id const &diagram)
	{
		emit diagramClosed(diagram);
	}

	void emitCodeTabClosed(QFileInfo const &fileInfo)
	{
		emit codeTabClosed(fileInfo);
	}
};
}
