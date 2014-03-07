#pragma once

#include <QtCore/QObject>
#include <QtCore/QFileInfo>
#include <QtWidgets/QPlainTextEdit>

#include <qrkernel/ids.h>

namespace qReal
{

/// Provides global singnals
class SystemEventsInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~SystemEventsInterface() {}

	virtual void emitCloseMainWindow() = 0;

	virtual void emitActiveTabChanged(Id const & rootElementId) = 0;

	virtual void emitSettingsUpdated() = 0;

	virtual void emitCodePathChanged(qReal::Id const &diagram
			, QFileInfo const &oldFileInfo
			, QFileInfo const &newFileInfo) = 0;

	virtual void emitNewCodeAppeared(qReal::Id const &diagram, QFileInfo const &fileInfo) = 0;

	virtual void emitDiagramClosed(qReal::Id const &diagram) = 0;

	virtual void emitCodeTabClosed(QFileInfo const &fileInfo) = 0;

signals:
	void closedMainWindow();
	void activeTabChanged(Id const & rootElementId);
	void settingsUpdated();
	void codePathChanged(qReal::Id const &diagram, QFileInfo const &oldFileInfo, QFileInfo const &newFileInfo);
	void newCodeAppeared(qReal::Id const &diagram, QFileInfo const &fileInfo);
	void diagramClosed(qReal::Id const &diagram);
	void codeTabClosed(QFileInfo const &fileInfo);
};
}
