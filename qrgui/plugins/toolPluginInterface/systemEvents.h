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
};

}
