#pragma once

#include <QtCore/QObject>
#include <QtCore/QFileInfo>
#include <QtWidgets/QPlainTextEdit>

#include <qrkernel/ids.h>

#include "qrgui/plugins/toolPluginInterface/toolPluginInterfaceDeclSpec.h"

namespace qReal {

class QRGUI_TOOL_PLUGIN_INTERFACE_EXPORT SystemEvents : public QObject
{
	Q_OBJECT

signals:
	void closedMainWindow();
	void activeTabChanged(Id const &rootElementId);
	void settingsUpdated();
	void codePathChanged(qReal::Id const &diagram, QFileInfo const &oldFileInfo, QFileInfo const &newFileInfo);
	void newCodeAppeared(qReal::Id const &diagram, QFileInfo const &fileInfo);
	void diagramClosed(qReal::Id const &diagram);
	void codeTabClosed(QFileInfo const &fileInfo);

	/// Emitted each time when new element was added into the logical model.
	void logicalElementAdded(Id const &id);
	/// Emitted each time when new element was added into the graphical model.
	void graphicalElementAdded(Id const &id);

	/// Emitted when new message with level 'Info' added to error reporter.
	void informationAdded(QString const &message, Id const &position);
	/// Emitted when new message with level 'Warning' added to error reporter.
	void warningAdded(QString const &message, Id const &position);
	/// Emitted when new message with level 'Error' added to error reporter.
	void errorAdded(QString const &message, Id const &position);
	/// Emitted when new message with level 'Critical' added to error reporter.
	void criticalAdded(QString const &message, Id const &position);

	/// This event may be subscribed to obtain all low-level user actions, timer events and everything about
	/// application`s event propagation subsystem. This is very powerful and dangerous tool, use it with care,
	/// it can strongly harm application`s performance.
	void lowLevelEvent(QObject *obj, QEvent *e);
};

}
