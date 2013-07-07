#pragma once

#include <QtCore/QObject>
#include <QtCore/QFileInfo>
#include <QtWidgets/QPlainTextEdit>

#include "../textEditor/codeArea.h"

namespace qReal
{

class Id;

class SystemEventsInterface : public QObject
{
	Q_OBJECT

public:
	virtual ~SystemEventsInterface() {}

	virtual void emitCloseMainWindow() = 0;

	virtual void emitActiveTabChanged(Id const & rootElementId) = 0;

	virtual void emitSettingsUpdated() = 0;

	virtual void emitCodePathChanged(CodeArea *area, QFileInfo const &fileInfo) = 0;

signals:
	void closedMainWindow();
	void activeTabChanged(Id const & rootElementId);
	void settingsUpdated();
	void codePathChanged(CodeArea *area, QFileInfo const &fileInfo);
};
}
