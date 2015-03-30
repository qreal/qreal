#pragma once

#include <QtCore/QObject>
#include <QtCore/QFileInfo>
#include <QtWidgets/QPlainTextEdit>

#include <qrkernel/ids.h>

#include "qrgui/plugins/toolPluginInterface/toolPluginInterfaceDeclSpec.h"
#include "qrgui/plugins/toolPluginInterface/tabInfo.h"

namespace qReal {

class QRGUI_TOOL_PLUGIN_INTERFACE_EXPORT SystemEvents : public QObject
{
	Q_OBJECT

signals:
	void closedMainWindow();
	void activeTabChanged(const TabInfo &info);
	void settingsUpdated();
	void codePathChanged(const qReal::Id &diagram, const QFileInfo &oldFileInfo, const QFileInfo &newFileInfo);
	void newCodeAppeared(const qReal::Id &diagram, const QFileInfo &fileInfo);
	void diagramClosed(const qReal::Id &diagram);
	void codeTabClosed(const QFileInfo &fileInfo);

	/// Emitted each time when new element was added into the logical model.
	void logicalElementAdded(const Id &id);
	/// Emitted each time when new element was added into the graphical model.
	void graphicalElementAdded(const Id &id);

	/// Emitted when new message with level 'Info' added to error reporter.
	void informationAdded(const QString &message, const Id &position);
	/// Emitted when new message with level 'Warning' added to error reporter.
	void warningAdded(const QString &message, const Id &position);
	/// Emitted when new message with level 'Error' added to error reporter.
	void errorAdded(const QString &message, const Id &position);
	/// Emitted when new message with level 'Critical' added to error reporter.
	void criticalAdded(const QString &message, const Id &position);

	/// This event may be subscribed to obtain all low-level user actions, timer events and everything about
	/// application`s event propagation subsystem. This is very powerful and dangerous tool, use it with care,
	/// it can strongly harm application`s performance.
	void lowLevelEvent(QObject *obj, QEvent *e);
};

}
